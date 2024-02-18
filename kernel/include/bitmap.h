//
// Created by huxulion on 2024/2/18.
//

#ifndef AOS_BITMAP_H
#define AOS_BITMAP_H

#include "types.h"

struct bitmap {
    uint32_t byte_size;
    uint8_t *bits;          // start address
};

typedef struct bitmap bitmap_t;

#endif //AOS_BITMAP_H
