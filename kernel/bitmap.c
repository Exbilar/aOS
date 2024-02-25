//
// Created by huxulion on 2024/2/18.
//

#include "include/bitmap.h"
#include "include/string.h"
#include "include/assert.h"

void init_bitmap(bitmap_t *btmap, uint32_t byte_size) {
    btmap->byte_size = byte_size;
    memset(btmap->bits, 0, btmap->byte_size);
}

bool bitmap_scan_test(bitmap_t *btmap, uint32_t bit_index) {
    uint32_t byte_index = bit_index >> 3;
    uint8_t pos = bit_index & 0x07;
    uint8_t bit = btmap->bits[byte_index];
    return ((bit >> pos) & 1) ? true : false;
}

void bitmap_bit_set(bitmap_t *btmap, uint32_t bit_index, uint val) {
    assert_write(val == 0 || val == 1, "panic bitmap_bit_set: invalid val");
    uint32_t byte_index = bit_index >> 3;
    uint8_t mask = (1 << (bit_index & 0x07));
    uint8_t *bit = &btmap->bits[byte_index];
    switch (val) {
        case 0:
            *bit &= ~mask;
            break;
        case 1:
            *bit |= mask;
            break;
        default:
            break;
    }
}

uint32_t bitmap_scan(bitmap_t *btmap, uint32_t bit_cnt) {
    uint32_t byte_idx = 0;
    while (btmap->bits[byte_idx] == 0xff && byte_idx < btmap->byte_size) {
        byte_idx++;
    }

    assert_write(byte_idx <= btmap->byte_size, "panic bitmap_scan: byte_idx overflow");
    if (byte_idx == btmap->byte_size) {
        return -1;
    }

    int bit_idx = 0;
    while ((uint8_t) (1 << bit_idx) & btmap->bits[byte_idx]) {
        bit_idx++;
    }
    uint32_t bit_idx_start = byte_idx * 8 + bit_idx;
    if (bit_cnt == 1) return bit_idx_start;

    uint32_t bit_left = (btmap->byte_size * 8 - bit_idx_start);
    uint32_t next_bit = bit_idx_start + 1;
    uint32_t count = 1;

    bit_idx_start = -1;
    while (bit_left-- > 0) {
        if (!(bitmap_scan_test(btmap, next_bit))) {
            count++;
        }
        else {
            count = 0;
        }
        if (count == bit_cnt) {
            bit_idx_start = next_bit - bit_cnt + 1;
            break;
        }
        next_bit++;
    }
    return bit_idx_start;
}