//
// Created by huxulion on 2024/2/23.
//

#ifndef AOS_DIR_H
#define AOS_DIR_H

#include "types.h"
#include "fs.h"

#define MAX_FILE_NAME 16

struct dir {
    struct inode *inode;
    uint32_t dir_pos;
    uint8_t dir_buf[512];
};

struct dir_entry {
    char file_name[MAX_FILE_NAME];
    uint32_t i_no;
    enum file_type f_type;
};

#endif //AOS_DIR_H
