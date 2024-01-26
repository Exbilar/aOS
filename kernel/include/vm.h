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

struct vaddr{
    uint32_t mem_start;
    struct spinlock lock;
    struct run *freelist;
};

typedef uint32_t* pagetable_t;

static pagetable_t const PDT_START = (pagetable_t) 0x400000;
static pagetable_t const PDT_END = (pagetable_t) 0x401000;

void free_page(char *pa);
void free_PDT();
void pagetable_init();
void load_pagetable();
void page_enable();

void mappage(uint32_t PDX, uint32_t PTX, uint32_t pa);
void vinit();
void* valloc();
void vfree(char *va);
void* malloc_page();
void mfree_page(void *va);
uint32_t addr_v2p(uint32_t va);

#endif //AOS_VM_H
