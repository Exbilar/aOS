//
// Created by 胡旭林 on 2023/7/7.
//

#include "include/gdt.h"

struct segdesc gdt[NSEGS];

uint64_t create_desc(uint32_t base, uint32_t limit, uint16_t flag) {
    uint64_t descriptor;

    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24

    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;

    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0

    return descriptor;
}

void set_gdt_entry(int index, uint64_t desc) {
    uint64_t *p = (uint64_t*) gdt;
    p[index] = desc;
}

void load_gdt(void) {
    lgdt(gdt, NSEGS * sizeof(gdt));
}

void init_gdt(void) {
    uint64_t desc;

    desc = create_desc(0, 0, 0);
    set_gdt_entry(0, desc);

    desc = create_desc(0, 0x000FFFFF, (GDT_CODE_PL0));
    set_gdt_entry(1, desc);

    desc = create_desc(0, 0x000FFFFF, (GDT_DATA_PL0));
    set_gdt_entry(2, desc);

    desc = create_desc(0, 0x000FFFFF, (GDT_CODE_PL3));
    set_gdt_entry(3, desc);

    desc = create_desc(0, 0x000FFFFF, (GDT_DATA_PL3));
    set_gdt_entry(4, desc);
}