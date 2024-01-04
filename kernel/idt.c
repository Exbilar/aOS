//
// Created by 胡旭林 on 2023/7/10.
//

#include "include/idt.h"
#include "include/vga.h"

struct gatedesc idt[NINTS];

uint32_t set_selector(uint rpl, uint ti, uint index) {
    return (uint32_t) ((index << 3) | (rpl << 1) | (ti));
}

uint64_t create_idt_entry(uint32_t off, uint16_t sel, uint d) {
    struct gatedesc desc;
    SETGATE(desc, 0, sel, off, d);
    uint64_t* res = (uint64_t*) &desc;
    return *res;
}

void set_idt_entry(uint64_t desc, uint index) {
    uint64_t *p = (uint64_t*) (idt);
    p[index] = desc;
}

void register_handler(void (*handler)(), uint vec) {
    uint32_t sel = set_selector(0, 0, 1);
    uint64_t desc = create_idt_entry((uint32_t) handler, sel, 0);
    set_idt_entry(desc, vec);
}

void load_idt() {
    lidt(idt, sizeof(idt));
}

void time_handler() {
    terminal_write("time");
}

void init_idt() {
    for (uint i = 32; i < 256; i++) {
        register_handler(&time_handler, i);
    }
    load_idt();
    terminal_write("idt init done");
}