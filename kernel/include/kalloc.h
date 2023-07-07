//
// Created by 胡旭林 on 2023/7/4.
//

#ifndef AOS_KALLOC_H
#define AOS_KALLOC_H

#include <stdint.h>

void freerange(void *vstart, void *vend);
void kinit(void *vstart, void *vend);
void* kalloc(void);
void kfree(char *v);

#endif //AOS_KALLOC_H
