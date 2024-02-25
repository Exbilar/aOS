//
// Created by huxulion on 2024/2/23.
//

#ifndef AOS_INODE_H
#define AOS_INODE_H

#include "types.h"
#include "list.h"

struct inode {
    uint32_t i_no;

    // i_size value depends on the type of the inode.
    // if directory, it represents the value of the sum of the file in this directory, or the size of the file that
    // this inode points to
    uint32_t i_size;
    uint32_t i_open_cnt;
    bool write_deny;        // check this before write

    // 0 ~ 11th is direct blocks, and 12th is a 1 level indirect block
    uint32_t i_sectors[13];
    struct list_ele inode_tag;
};

#endif //AOS_INODE_H
