//
// Created by 胡旭林 on 2024/1/14.
//

#include "include/irq.h"
#include "include/idt.h"
#include "include/i386.h"
#include "include/vga.h"

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
    outb(PIC_M_DATA, 0xfe);
    outb(PIC_S_DATA, 0xff);
}

void init_intr() {
    init_idt();
    irq_remap();
    sti();
}

extern void intr_exit();

void irq_handler(struct regs *r) {

    if (r->int_no < 32) {
        terminal_write(exception_messages[r->int_no]);
        for (;;);
    }

    if (r->int_no >= 40) {
        outb(PIC_S_CTRL, 0x20);
    }

    outb(PIC_M_CTRL, 0x20);

    if (r->int_no == 32) {
        terminal_write("Timer Interrupt!");
    }

}