//
// Created by huxulion on 2024/1/31.
//

#include "include/syscall.h"

extern void* handler_entry_list[];

int syscall_handler(struct regs *r) {
    return 0xff;
}