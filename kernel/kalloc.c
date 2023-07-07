//
// Created by 胡旭林 on 2023/7/5.
//

#include "include/kalloc.h"
#include "include/mmu.h"
#include "include/stdio.h"
#include "include/i386.h"

struct run {
    struct run *next;
};

struct {
    uint32_t mem_low;
    uint32_t mem_high;
    struct run *freelist;
}kmem;

void kfree(char *v) {
    struct run *r;
    if(((uint32_t)v % PGSIZE) != 0 || (uint32_t)v < kmem.mem_low || (uint32_t)v >= kmem.mem_high)
        panic("kfree");
    memset(v, 1, PGSIZE);

    r = (struct run*) v;

    cli();
    r->next = kmem.freelist;
    kmem.freelist = r;
    sti();
}

void kinit(void *vstart, void *vend) {
    kmem.mem_low = (uint32_t) vstart;
    kmem.mem_high = (uint32_t) vend;
    freerange(vstart, vend);
}

void freerange(void *vstart, void *vend) {
    char *p;
    p = (char*)PGROUNDUP((uint32_t) vstart);
    for (; p + PGSIZE <= (char*)vend; p += PGSIZE) {
        kfree(p);
    }
}

void* kalloc(void) {
    struct run *r;

    cli();
    r = kmem.freelist;
    if (r)
        kmem.freelist = r->next;
    sti();

    if (r)
        memset((char*)r, 5, PGSIZE);
    return (void*)r;
}