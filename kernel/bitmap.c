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

int bitmap_scan(bitmap_t *btmap, uint32_t bit_cnt) {

}