//
// Created by huxulion on 2024/2/14.
//

#include "include/ide.h"
#include "include/timer.h"
#include "include/sleeplock.h"
#include "include/irq.h"
#include "include/kalloc.h"

#define reg_data(channel)           (channel->port_base + 0)
#define reg_err(channel)            (channel->port_base + 1)
#define reg_sect_cnt(channel)       (channel->port_base + 2)
#define reg_lba_l(channel)          (channel->port_base + 3)
#define reg_lba_m(channel)          (channel->port_base + 4)
#define reg_lba_h(channel)          (channel->port_base + 5)
#define reg_dev(channel)            (channel->port_base + 6)
#define reg_status(channel)         (channel->port_base + 7)
#define reg_cmd(channel)            (reg_status(channel))
#define reg_altne_status(channel)   (channel->port_base + 0x206)
#define reg_ctl(channel)            (reg_alte_status(channel))

#define BIT_STAT_BUSY               0x80
#define BIT_STAT_DRDY               0x40
#define BIT_STAT_DREQ               0x08
#define BIT_DEV_MBS                 0xa0
#define BIT_DEV_LBA                 0x40
#define BIT_DEV_DEV                 0x10

#define CMD_IDENTIFY                0xec
#define CMD_READ_SECTOR             0x20
#define CMD_WRITE_SECTOR            0x30

#define MAX_LBA                     ((128 * 1024 * 1024 / 512) - 1)

struct partition_table_entry {
    uint8_t bootable;
    uint8_t start_head;
    uint8_t boot_sector;
    uint8_t start_CHS;
    uint8_t fs_type;
    uint8_t end_head;
    uint8_t end_sector;
    uint8_t end_CHS;

    uint32_t start_lba;
    uint32_t sector_cnt;
} __attribute__((packed));

struct boot_sector {
    uint8_t other[446];
    struct partition_table_entry partition_table[4];
    uint16_t signature;
} __attribute__((packed));

uint8_t channel_cnt;
struct ide_channel channels[2];

int32_t ext_lba_base = 0;
uint8_t p_no = 0, l_no = 0;

struct list partition_list;

static void partition_scan(struct disk *dsk, uint32_t ext_lba);

void ide_init() {
    uint8_t hd_cnt = *((uint8_t*)(0x475));
    assert(hd_cnt > 0);

    channel_cnt = (hd_cnt + 1) / 2;
    assert(channel_cnt < 2);

    list_init(&partition_list);
    for (int channel_no = 0; channel_no < channel_cnt; channel_no++) {
        struct ide_channel *channel = &channels[channel_no];
        switch (channel_no) {
            case 0:
                channel->port_base = 0x1f0;
                channel->irq_no = 0x20 + 14;
                break;
            case 1:
                channel->port_base = 0x170;
                channel->irq_no = 0x20 + 15;
                break;
        }
        channel->expecting_intr = false;
        init_lock(&channel->mutex_lock, "channel_mutex_lock");
        init_semaphore(&channel->sem, 0);

        irq_install(channel->irq_no, intr_disk_handler);

        uint dev_no = 0;
        while (dev_no < 2) {
            struct disk *hd = &channel->devices[dev_no];
            strcpy("disk", hd->name, strlen("disk"));
            hd->channel = channel;
            hd->dev_no = dev_no;
            if (dev_no == 0) {          // only one hard disk
                partition_scan(hd, 0);
            }
            p_no = 0, l_no = 0;
            dev_no++;
        }
    }
}

static void select_disk(struct disk *dsk) {
    uint8_t reg_device = BIT_DEV_MBS | BIT_DEV_LBA;
    if (dsk->dev_no == 1) {
        reg_device |= BIT_DEV_DEV;
    }
    outb(reg_dev(dsk->channel),reg_device);
}

static void select_sector(struct disk *dsk, uint32_t lba, uint32_t sect_cnt) {
    assert(lba <= MAX_LBA);
    struct ide_channel *my_channel = dsk->channel;

    outb(reg_sect_cnt(my_channel), sect_cnt);

    // writing lba to channel port
    outb(reg_lba_l(my_channel), lba & 0xff);
    outb(reg_lba_m(my_channel), (lba >> 8) & 0xff);
    outb(reg_lba_h(my_channel), (lba >> 16) & 0xff);
    outb(reg_dev(my_channel), BIT_DEV_MBS | BIT_DEV_LBA |
        (dsk->dev_no == 1 ? BIT_DEV_DEV : 0) | (lba >> 24));
}

static void cmd_out(struct ide_channel *channel, uint8_t cmd) {
    channel->expecting_intr = true;
    outb(reg_cmd(channel), cmd);
}

static void read_from_sector(struct disk *dsk, void *buf, uint8_t sect_cnt) {
    uint32_t byte_size = (sect_cnt == 0 ? 256 * 512 : sect_cnt * 512);
    insw(reg_data(dsk->channel), buf, byte_size >> 1);
}

static void write2sector(struct disk *dsk, void *buf, uint8_t sect_cnt) {
    uint32_t byte_size = (sect_cnt == 0 ? 256 * 512 : sect_cnt * 512);
    outsw(reg_data(dsk->channel), buf, byte_size >> 1);
}

static bool disk_wait(struct disk *dsk) {
    struct ide_channel *my_channel = dsk->channel;
    uint32_t time_limit = 30 * 1000;                 // 30s time_limit (1000Hz CPU)
    while (time_limit -= 10 >= 0) {
        if (!(inb(reg_status(my_channel)) & BIT_STAT_BUSY)) {
            return (inb(reg_status(my_channel)) & BIT_STAT_DREQ);
        }
        else {
            timer_wait(10);
        }
    }
    return false;
}

void ide_read(struct disk *dsk, uint32_t lba, void *buf, uint32_t sect_cnt) {
    assert_write(lba <= MAX_LBA, "panic ide_read: lba size exceeded");
    assert_write(sect_cnt > 0, "panic ide_read: invalid sect_cnt size");

    acquire(&dsk->channel->mutex_lock);

    select_disk(dsk);

    uint32_t sect_op;
    uint32_t sect_done = 0;

    while (sect_done < sect_cnt) {
        if ((sect_done + 256) <= sect_cnt) {
            sect_op = 256;
        }
        else {
            sect_op = sect_cnt - sect_done;
        }
        select_sector(dsk, lba + sect_done, sect_op);
        cmd_out(dsk->channel, CMD_READ_SECTOR);

        semaphore_P(&dsk->channel->sem);

        if (!disk_wait(dsk)) {
            panic("panic ide_read: disk_wait error");
        }

        read_from_sector(dsk, (void*)((uint32_t)buf + sect_done * 512), sect_op);
        sect_done += sect_op;
    }

    release(&dsk->channel->mutex_lock);
}

void ide_write(struct disk *dsk, uint32_t lba, void *buf, uint32_t sect_cnt) {
    assert_write(lba <= MAX_LBA, "panic ide_write: lba size exceeded");
    assert_write(sect_cnt > 0, "panic ide_write: invalid sect_cnt size");

    acquire(&dsk->channel->mutex_lock);

    select_disk(dsk);

    uint32_t sect_op;
    uint32_t sect_done = 0;

    while (sect_done < sect_cnt) {
        if ((sect_done + 256) <= sect_cnt) {
            sect_op = 256;
        }
        else {
            sect_op = sect_cnt - sect_done;
        }
        select_sector(dsk, lba + sect_done, sect_op);
        cmd_out(dsk->channel, CMD_WRITE_SECTOR);

        if (!disk_wait(dsk)) {
            panic("panic ide_write: disk_wait error");
        }
        write2sector(dsk, (void*)((uint32_t)buf + sect_done * 512), sect_op);
        semaphore_P(&dsk->channel->sem);
        sect_done += sect_op;
    }

    release(&dsk->channel->mutex_lock);
}

void intr_disk_handler(struct regs *r) {
    uint32_t irq_no = r->int_no;
    assert_write(irq_no == 0x2e || irq_no == 0x2f, "panic intr_disk_handler: invalid irq_no");

    uint8_t ch_no = irq_no - 0x2e;
    struct ide_channel *channel = &channels[ch_no];

    assert_write(irq_no == channel->irq_no, "panic intr_disk_handler: incompatible irq_no");

    if (channel->expecting_intr) {
        channel->expecting_intr = false;
        semaphore_V(&channel->sem);
        inb(reg_status(channel));
    }
}

static void partition_scan(struct disk *dsk, uint32_t ext_lba) {
    struct boot_sector *bs = kalloc();
    ide_read(dsk, ext_lba, bs, 1);

    uint8_t part_idx = 0;
    struct partition_table_entry *p = bs->partition_table;

    while (part_idx++ < 4) {
        if (p->fs_type == 0x5) {
            if (ext_lba_base != 0) {
                partition_scan(dsk, p->start_lba + ext_lba_base);
            }
            else {
                ext_lba_base = p->start_lba;
                partition_scan(dsk, p->start_lba);
            }
        }
        else if (p->fs_type != 0) {
            if (ext_lba == 0) {
                dsk->prim_parts[p_no].start_lba = ext_lba + p->start_lba;
                dsk->prim_parts[p_no].sec_cnt = p->sector_cnt;
                dsk->prim_parts[p_no].dsk = dsk;
                list_insert_rear(&partition_list, &dsk->prim_parts[p_no].part_tag);
                p_no++;
            }
            else {
                dsk->logic_parts[l_no].start_lba = ext_lba + p->start_lba;
                dsk->logic_parts[l_no].sec_cnt = p->sector_cnt;
                dsk->logic_parts[l_no].dsk = dsk;
                list_insert_rear(&partition_list, &dsk->logic_parts[l_no].part_tag);
                l_no++;
                if (l_no >= 8) return ;
            }
        }
        p++;
    }

    kfree((char*) bs);
}
