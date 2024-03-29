//
// Created by huxulion on 2024/1/31.
//

#include "include/syscall.h"
#include "include/vga.h"
#include "include/thread.h"
#include "include/timer.h"

enum SYSCALL_NR {
    SYS_GETPID,
    SYS_WRITE,
    SYS_SLEEP
};

void *syscall_list[127] = {0};

static int sys_write(struct regs *r) {
    char *str = (char *)r->ebx;
    terminal_write(str);
    return (int) strlen(str);
}

static int sys_getpid(struct regs *r) {
    thread_t *pthread = running_thread();
    return pthread->pid;
}

static int sys_sleep(struct regs *r) {
    timer_wait(r->ebx);
    return r->ebx;
}

void syscall_install(int sys_no, void *handler) {
    syscall_list[sys_no] = handler;
}

int syscall_handler(struct regs *r) {
    int sys_no = r->eax;
    int (*handler) (struct regs *r);
    handler = syscall_list[r->eax];
    if (handler) {
        return handler(r);
    }
    return 0xfecb;          // magic number
}

int getpid() {
    return _syscall0(SYS_GETPID);
}

int write(char *str) {
    return _syscall1(SYS_WRITE, str);
}

int sleep(int ticks) {
    return _syscall1(SYS_SLEEP, ticks);
}

void syscall_init() {
    syscall_install(SYS_GETPID, sys_getpid);
    syscall_install(SYS_WRITE, sys_write);
    syscall_install(SYS_SLEEP, sys_sleep);
}