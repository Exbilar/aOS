//
// Created by huuxlion on 2023/7/4.
//

#ifndef AOS_KALLOC_H
#define AOS_KALLOC_H

#include <stdint.h>
#include "mmu.h"
#include "i386.h"
#include "lock.h"
#include "assert.h"

#define NPAGES 1024 * 1024
#define PHYSIZE 128 * 1024 * 1024       // physical memory size

struct run {
    struct run *next;
};

void freerange(void *pa_start, void *pa_end);
void kinit(void *pa_start, void *pa_end);
void* kalloc();
void kfree(char *pa);

#endif //AOS_KALLOC_H
