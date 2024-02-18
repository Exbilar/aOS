//
// Created by huxulion on 2024/2/12.
//

#ifndef AOS_IDE_H
#define AOS_IDE_H

#include "types.h"
#include "list.h"
#include "lock.h"
#include "sleeplock.h"
#include "irq.h"
#include "bitmap.h"
#include "super_block.h"

struct partition {
    char name[16];
    uint32_t start_lba;
    uint32_t sec_cnt;
    struct disk *dsk;
    struct super_block *sb;
    bitmap_t block_bitmap;
    bitmap_t inode_bitmap;
    list_t   open_inodes;
};

// hdisk.img has only one partition,
// but here still comes with 4 primary partitions and 8 logic partitions.
struct disk {
    char name[16];
    struct ide_channel *channel;
    uint8_t dev_no;
    struct partition prim_parts[4];
    struct partition logic_parts[8];
};

// structure of ATA channel, which manages disk devices.
struct ide_channel {
    char name[16];
    uint16_t port_base;
    uint8_t irq_no;                 // irq number of this channel
    struct spinlock mutex_lock;     // mutex lock used for mutex access of channel
    struct semaphore sem;           // semaphore used for disk operations
    bool expecting_intr;
    struct disk devices[2];         // master-slave disks
};

void ide_init();
void intr_disk_handler(struct regs *r);
void ide_read(struct disk *dsk, uint32_t lba, void *buf, uint32_t sect_cnt);
void ide_write(struct disk *dsk, uint32_t lba, void *buf, uint32_t sect_cnt);

#endif //AOS_IDE_H
