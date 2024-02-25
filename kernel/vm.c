//
// Created by huuxlion on 2024/1/18.
//

#include "include/vm.h"

struct vaddr vmem;

static void free_PDT() {
    memset((char *)PDT_START, 0, PGSIZE);
}

static void load_pagetable() {
    lcr3((uint32_t)PDT_START);
}

static void page_enable() {
    uint32_t val = rcr0();
    val |= CR0_PG;
    lcr0(val);
}

void map_kernel_pages(uint32_t start, uint32_t end) {
    for (uint32_t va = start; va + PGSIZE <= end; va += PGSIZE) {
        kernel_mappage(PDX(va), PTX(va), PGADDR(PDX(va), PTX(va), 0));
    }
}

void pagetable_init() {
    free_PDT();
    map_kernel_pages(0, PHYSIZE);     // identity map 0 ~ 128MiB physical memory to itself
    load_pagetable();
    page_enable();
}

void kernel_mappage(uint32_t PDX, uint32_t PTX, uint32_t pa) {
    pagetable_t PDE = &PDT_START[PDX];
    if (*PDE == 0) {
        *PDE = (uint32_t) (PDT_END + PDX * PGSIZE) | PTE_P | PTE_U | PTE_W;
    }
    assert_write(*PDE != 0, "panic: kernel_mappage");
    pagetable_t PTT = (pagetable_t) (PTE_ADDR(*PDE));
    pagetable_t PTE = &PTT[PTX];
    *PTE = pa | PTE_P | PTE_U | PTE_W;
}

void mappage(pagetable_t pgdir, uint32_t va, uint32_t pa) {
    pagetable_t PDE = &pgdir[PDX(va)];
    if (*PDE == 0) {
        uint32_t alloc = (uint32_t) kalloc();
        *PDE = alloc | PTE_P | PTE_U | PTE_W;
    }
    assert_write(*PDE != 0, "panic mappage");
    pagetable_t PTT = (pagetable_t) (PTE_ADDR(*PDE));
    pagetable_t PTE = &PTT[PTX(va)];
    *PTE = pa | PTE_P | PTE_U | PTE_W;
}