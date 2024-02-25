//
// Created by huxulion on 2024/2/23.
//

#include "include/fs.h"
#include "include/inode.h"
#include "include/ide.h"

#define DIV_ROUND_UP(x, y) ((x) + ((y) - 1)) / (y)

static void partition_format(struct disk *hd, struct partition *part) {
    uint32_t boot_sector_sects = 1;
    uint32_t super_block_sects = 1;
    uint32_t inode_bitmap_sects =
            DIV_ROUND_UP(MAX_FILES_PER_PARTITION, SECTOR_BIT_SIZE);
    uint32_t inode_table_sects =
            DIV_ROUND_UP((sizeof(struct inode) * MAX_FILES_PER_PARTITION), SECTOR_SIZE);
    uint32_t used_sects = boot_sector_sects + super_block_sects +
            inode_bitmap_sects + inode_table_sects;
    uint32_t free_sects = part->sec_cnt - used_sects;

    uint32_t block_bitmap_sects;
    block_bitmap_sects = DIV_ROUND_UP(free_sects, SECTOR_BIT_SIZE);
    uint32_t block_bitmap_bit_len = free_sects - block_bitmap_sects;
    block_bitmap_sects = DIV_ROUND_UP(block_bitmap_bit_len, SECTOR_BIT_SIZE);

    struct super_block sb;
    sb.magic = 0x1234567;
    sb.sec_cnt = part->sec_cnt;
    sb.inode_cnt = MAX_FILES_PER_PARTITION;
    sb.part_lba_base = part->start_lba;

}