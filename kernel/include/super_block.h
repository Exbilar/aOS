//
// Created by huxulion on 2024/2/18.
//

#ifndef AOS_SUPER_BLOCK_H
#define AOS_SUPER_BLOCK_H

struct super_block {
    uint32_t magic;
    uint32_t sec_cnt;
    uint32_t inode_cnt;
    uint32_t part_lba_base;

    uint32_t block_bitmap_lba;
    uint32_t block_bitmap_sects;

    uint32_t inode_bitmap_lba;
    uint32_t inode_bitmap_sects;

    uint32_t inode_table_lba;
    uint32_t inode_table_sects;

    uint32_t data_start_lba;
    uint32_t root_inode_no;
    uint32_t dir_entry_size;

    uint8_t pad[460];
} __attribute__((packed));

#endif //AOS_SUPER_BLOCK_H
