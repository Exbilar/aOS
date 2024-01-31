//
// Created by huxulion on 2024/1/31.
//

#ifndef AOS_SYSCALL_H
#define AOS_SYSCALL_H

#include "irq.h"

int syscall_handler(struct regs *r);

#endif //AOS_SYSCALL_H
