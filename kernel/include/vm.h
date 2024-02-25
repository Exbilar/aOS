//
// Created by huuxlion on 2024/1/18.
//

#ifndef AOS_VM_H
#define AOS_VM_H

#include "types.h"
#include "mmu.h"
#include "i386.h"
#include "lock.h"
#include "string.h"
#include "kalloc.h"

#define VMEM_START 0xc00000

struct vaddr{
    uint32_t mem_start;
    struct spinlock lock;
    struct run *freelist;
};

typedef uint32_t* pagetable_t;

static pagetable_t const PDT_START = (pagetable_t) 0x400000;
static pagetable_t const PDT_END = (pagetable_t) 0x401000;

void pagetable_init();
void kernel_mappage(uint32_t PDX, uint32_t PTX, uint32_t pa);
void mappage(pagetable_t pgdir, uint32_t va, uint32_t pa);

#endif //AOS_VM_H
