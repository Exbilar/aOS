//
// Created by huuxlion on 2024/1/17.
//

#ifndef AOS_TIMER_H
#define AOS_TIMER_H

#include "irq.h"
#include "i386.h"

void timer_init();
void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

#endif //AOS_TIMER_H
