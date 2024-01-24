//
// Created by huuxlion on 2024/1/17.
//

#include "include/timer.h"
#include "include/vga.h"
#include "include/thread.h"
#include "include/stdio.h"
#include "include/i386.h"

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
    thread_t *pthread = running_thread();

    if (pthread->stack_magic != 0x12345678) {
        panic("panic: thread stack overflow");
    }

    pthread->elapsed_ticks++;
    if (pthread->ticks == 0) {
        schedule();
    }
    else {
        pthread->ticks--;
    }
}

void timer_wait(int ticks) {
    uint eflags = readeflags();
    assert_write(eflags & 0x00000200, "panic timer_wait: IF not set");
    uint32_t end_ticks = ticks + timer_ticks;
    while (timer_ticks < end_ticks);
}

void timer_install() {
    irq_install(0x20, timer_handler);
}