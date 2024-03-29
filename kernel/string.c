//
// Created by huuxlion on 2023/7/2.
//

#include "include/string.h"

size_t strlen(const char* str) {
    size_t res = 0;
    while (str[res]) res++;
    return res;
}

int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;
    for (int i = 0; i < size; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}

void strcpy(void *src, void *dst, size_t size) {
    unsigned char *src_ptr = (unsigned char *)src;
    unsigned char *dst_ptr = (unsigned char *)dst;
    for (size_t i = 0; i < size; i++) {
        *dst_ptr++ = *src_ptr++;
    }
}

void* memset(void* bufptr, int value, size_t size) {
    unsigned char* buf = (unsigned char*) bufptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = (unsigned char) value;
    return bufptr;
}
