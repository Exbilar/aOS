//
// Created by 胡旭林 on 2023/7/10.
//

#include "include/idt.h"
#include "include/irq.h"
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

void register_handler(void *handler, uint vec) {
    uint32_t sel = set_selector(0, 0, 1);
    uint64_t desc = create_idt_entry((uint32_t) handler, sel, 0);
    set_idt_entry(desc, vec);
}

void load_idt() {
    lidt(idt, sizeof(idt));
}

void init_idt() {
    register_handler(irq_0, 0);
    register_handler(irq_1, 1);
    register_handler(irq_2, 2);
    register_handler(irq_3, 3);
    register_handler(irq_4, 4);
    register_handler(irq_5, 5);
    register_handler(irq_6, 6);
    register_handler(irq_7, 7);
    register_handler(irq_8, 8);
    register_handler(irq_9, 9);
    register_handler(irq_10, 10);
    register_handler(irq_11, 11);
    register_handler(irq_12, 12);
    register_handler(irq_13, 13);
    register_handler(irq_14, 14);
    register_handler(irq_15, 15);
    register_handler(irq_16, 16);
    register_handler(irq_17, 17);
    register_handler(irq_18, 18);
    register_handler(irq_19, 19);
    register_handler(irq_20, 20);
    register_handler(irq_21, 21);
    register_handler(irq_22, 22);
    register_handler(irq_23, 23);
    register_handler(irq_24, 24);
    register_handler(irq_25, 25);
    register_handler(irq_26, 26);
    register_handler(irq_27, 27);
    register_handler(irq_28, 28);
    register_handler(irq_29, 29);
    register_handler(irq_30, 30);
    register_handler(irq_31, 31);

    register_handler(irq_32, 32);
    register_handler(irq_33, 33);
    register_handler(irq_34, 34);
    register_handler(irq_35, 35);
    register_handler(irq_36, 36);
    register_handler(irq_37, 37);
    register_handler(irq_38, 38);
    register_handler(irq_39, 39);
    register_handler(irq_40, 40);
    register_handler(irq_41, 41);
    register_handler(irq_42, 42);
    register_handler(irq_43, 43);
    register_handler(irq_44, 44);
    register_handler(irq_45, 45);
    register_handler(irq_46, 46);
    register_handler(irq_47, 47);

    load_idt();
    terminal_write("idt init done");
}