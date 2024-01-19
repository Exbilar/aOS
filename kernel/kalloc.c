//
// Created by 胡旭林 on 2023/7/5.
//

#include "include/kalloc.h"

struct {
    uint32_t mem_low;
    uint32_t mem_high;
    struct run *freelist;
}kmem;

void kinit(void *pa_start, void *pa_end) {
    kmem.mem_low = (uint32_t) pa_start;
    kmem.mem_high = (uint32_t) pa_end;
    kmem.freelist = NULL;
    freerange(pa_start, pa_end);
}

void freerange(void *pa_start, void *pa_end) {
    char *p = (char *) PGROUNDUP((uint32_t) pa_start);
    for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE) {
        kfree(p);
    }
}

void* kalloc() {
    cli();
    struct run* r = kmem.freelist;
    if (r) {
        kmem.freelist = kmem.freelist->next;
    }
    sti();
    if (r) {
        memset((char *)r, 0, PGSIZE);
    }
    return (void *)r;
}

void kfree(char *pa) {
    char *p = (char *) PGROUNDDOWN((uint32_t)pa);
    if (p < (char *)kmem.mem_low || p + PGSIZE > (char *)kmem.mem_high)
        panic("kfree");

    struct run *r = (struct run*) p;
    cli();
    r->next = kmem.freelist;
    kmem.freelist = r;
    sti();
}