//
// Created by 胡旭林 on 2024/1/17.
//

#include "include/timer.h"
#include "include/vga.h"

int timer_ticks = 0;
int timer_hz = 100;

void timer_phase(int hz) {
    int div = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, div & 0xff);
    outb(0x40, div >> 8);
}

void timer_init() {
    timer_ticks = 0;
    timer_hz = 100;
    timer_phase(timer_hz);
    timer_install();
}

void timer_handler(struct regs *r) {
    timer_ticks++;
    if (timer_ticks % timer_hz == 0) {
        terminal_write("1 second passed");
    }
}

void timer_wait(int ticks) {
    uint32_t end_ticks = ticks + timer_ticks;
    while (timer_ticks < end_ticks);
}

void timer_install() {
    irq_install(0x20, timer_handler);
}