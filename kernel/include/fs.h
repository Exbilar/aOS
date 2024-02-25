//
// Created by huxulion on 2024/2/23.
//

#ifndef AOS_FS_H
#define AOS_FS_H

#define SECTOR_SIZE 512
#define SECTOR_BIT_SIZE 4096
#define MAX_FILES_PER_PARTITION 4096

enum file_type {
    FT_UNKNOWN,
    FT_REGULAR,
    FT_DIRECTORY
};

#endif //AOS_FS_H
