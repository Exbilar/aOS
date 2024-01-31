//
// Created by huuxlion on 2024/1/14.
//

#include "include/irq.h"
#include "include/idt.h"
#include "include/i386.h"
#include "include/vga.h"
#include "include/timer.h"

const unsigned char *exception_messages[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};

void* handler_entry_list[255] = {0};

void irq_install(int irq, void (*handler) (struct regs *r)) {
    handler_entry_list[irq] = handler;
}

void irq_uninstall(int irq) {
    handler_entry_list[irq] = 0;
}

void irq_remap() {
    outb(PIC_M_CTRL, 0x11);
    outb(PIC_M_DATA, 0x20);
    outb(PIC_M_DATA, 0x04);
    outb(PIC_M_DATA, 0x01);

    outb(PIC_S_CTRL, 0x11);
    outb(PIC_S_DATA, 0x28);
    outb(PIC_S_DATA, 0x02);
    outb(PIC_S_DATA, 0x01);

    // open timer interrupt only
    outb(PIC_M_DATA, 0xfc);
    outb(PIC_S_DATA, 0xff);
}

void init_intr() {
    init_idt();
    irq_remap();
    sti();
}

extern void intr_exit();

void irq_handler(struct regs *r) {

    // sending EOI signal to 8259A chip
    if (r->int_no >= 32) {
        outb(PIC_M_CTRL, 0x20);
    }
    if (r->int_no >= 40) {
        outb(PIC_S_CTRL, 0x20);
    }

    void (*handler) (struct regs *r);
    handler = handler_entry_list[r->int_no];
    if (handler) {
        handler(r);
    }

    if (r->int_no < 32) {
        terminal_write(exception_messages[r->int_no]);
        for (;;);
    }

}