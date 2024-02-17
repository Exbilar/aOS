//
// Created by huxulion on 2024/1/31.
//

#ifndef AOS_SYSCALL_H
#define AOS_SYSCALL_H

#include "irq.h"

#define _syscall0(NUM)                                                         \
  ({                                                                           \
    int res;                                                                   \
    asm volatile("int $0x80" : "=a"(res) : "a"(NUM) : "memory");               \
    res;                                                                       \
  })

#define _syscall1(NUM, ARG0)                                                   \
({                                                                             \
    int res;                                                                   \
    asm volatile("int $0x80"                                                   \
    : "=a"(res)                                                                \
    : "a"(NUM), "b"(ARG0)                                                      \
    : "memory");                                                               \
    res;                                                                       \
})

#define _syscall2(NUM, ARG0, ARG1)                                             \
({                                                                             \
    int res;                                                                   \
    asm volatile("int $0x80"                                                   \
    : "=a"(res)                                                                \
    : "a"(NUM), "b"(ARG0), "c"(ARG1)                                           \
    : "memory");                                                               \
    res;                                                                       \
})

#define _syscall3(NUMBER, ARG0, ARG1, ARG2)                                    \
  ({                                                                           \
    int res;                                                                   \
    asm volatile("int $0x80"                                                   \
    : "=a"(res)                                                                \
    : "a"(NUMBER), "b"(ARG0), "c"(ARG1), "d"(ARG2)                             \
    : "memory");                                                               \
    res;                                                                       \
  })

int syscall_handler(struct regs *r);

int getpid();
int write(char *str);
int sleep(int ticks);

void syscall_init();

#endif //AOS_SYSCALL_H
