//
// Created by huuxlion on 2024/1/18.
//

#include "include/vm.h"
#include "include/kalloc.h"

struct {
    uint32_t mem_start;
    struct spinlock lock;
    struct run *freelist;
}vmem;

void vinit() {
    vmem.mem_start = 0x300000;
    vmem.freelist = NULL;
    init_lock(&vmem.lock, "vmem");
}

void* valloc() {
    acquire(&vmem.lock);
    if (vmem.freelist  != NULL) {
        struct run *r = vmem.freelist;
        vmem.freelist = r->next;
        sti();
        return (void *)r;
    }
    uint32_t res = vmem.mem_start;
    vmem.mem_start += PGSIZE;
    release(&vmem.lock);
    return (void *)res;
}

void vfree(char *va) {
    va = (char *) PGROUNDDOWN((uint32_t)va);
    assert_write((uint32_t) va < vmem.mem_start, "panic: vfree");
    acquire(&vmem.lock);
    struct run *r = (struct run *)va;
    r->next = vmem.freelist;
    vmem.freelist = r;
    release(&vmem.lock);
}

void free_page(char *pa) {
    memset(pa, 0, PGSIZE);
}

void free_PDT() {
    memset((char *)PDT_START, 0, PGSIZE);
}

void load_pagetable() {
    lcr3((uint32_t)PDT_START);
}

void page_enable() {
    uint32_t val = rcr0();
    val |= CR0_PG;
    lcr0(val);
}

void map_kernel_pages(uint32_t start, uint32_t end) {
    for (uint32_t va = start; va + PGSIZE <= end; va += PGSIZE) {
        mappage(PDX(va), PTX(va), PGADDR(PDX(va), PTX(va), 0));
    }
}

void pagetable_init() {
    vinit();
    free_PDT();
    // identity map from 0 ~ 128MiB physical memory to itself
    map_kernel_pages(0, 0x8000000);
    load_pagetable();
    page_enable();
}

void mappage(uint32_t PDX, uint32_t PTX, uint32_t pa) {
    pagetable_t PDE = &PDT_START[PDX];
    if (*PDE == 0) {
        *PDE = (uint32_t) kalloc() | PTE_P | PTE_U | PTE_W;
    }
    assert_write(*PDE != 0, "panic: mappage");
    pagetable_t PTT = (pagetable_t) (PTE_ADDR(*PDE));
    pagetable_t PTE = &PTT[PTX];
    *PTE = pa | PTE_P | PTE_U | PTE_W;
}

void* malloc_page() {
    void *va = valloc();
    void *pa = kalloc();
    mappage(PDX(va), PTX(va), (uint32_t )pa);
    return va;
}

void mfree_page(void *va) {
    vfree(va);
}