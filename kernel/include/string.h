//
// Created by huuxlion on 2023/7/2.
//

#ifndef AOS_STRING_H
#define AOS_STRING_H

#include <stddef.h>

size_t strlen(const char* str);
int memcmp(const void* aptr, const void* bptr, size_t size);
void* memset(void* bufptr, int value, size_t size);
void strcpy(void *src, void *dst, size_t size);

#endif //AOS_STRING_H
