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

void bitmap_bit_set(bitmap_t *btmap, uint32_t bit_index, uint val);
bool bitmap_scan_test(bitmap_t *btmap, uint32_t bit_index);
void init_bitmap(bitmap_t *btmap, uint32_t byte_size);
uint32_t bitmap_scan(bitmap_t *btmap, uint32_t bit_cnt);

#endif //AOS_BITMAP_H
