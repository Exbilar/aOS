//
// Created by huuxlion on 2024/1/20.
//

#ifndef AOS_THREAD_H
#define AOS_THREAD_H

#include "mmu.h"
#include "vm.h"
#include "i386.h"
#include "list.h"
#include "lock.h"

typedef void thread_func(void *);

enum task_status {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_WAITING,
    TASK_HANGING,
    TASK_DIED
};

// struct thread_stack records the data structure of registers for scheduling
// func_addr and args_addr are used for function kernel_thread(thread_func* func, void *args)
struct thread_stack {
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edi;
    uint32_t esi;
    uint32_t eip;
    uint32_t unused_ret_addr;
    uint32_t func_addr;
    uint32_t args_addr;
};

typedef int16_t pid_t;

struct thread_state{
    uint32_t* thread_stack;
    pid_t pid;
    enum task_status status;
    uint8_t priority;
    uint8_t ticks;
    uint32_t elapsed_ticks;
    ele_t general_tag;
    ele_t blcok_tag;
    ele_t all_list_tag;
    uint32_t* pgdir;
    struct vaddr user_vmem;
    char name[16];
    uint32_t stack_magic;
};
typedef struct thread_state thread_t;

pid_t alloc_pid();
thread_t* running_thread();
void create_thread(thread_t* pthread, thread_func func, void* args);
void init_thread(thread_t *pthread, char *name, int prio);
void kernel_thread(thread_func* func, void *args);
thread_t* start_thread(char *name, int prio, thread_func func, void *args);

void schedule();
void enable_thread();

void thread_yield();

void activate_process(thread_t *pthread);

void task_block(enum task_status status);
void task_wakeup(thread_t *pthread);

#endif //AOS_THREAD_H
