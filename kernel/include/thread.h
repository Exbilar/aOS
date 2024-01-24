//
// Created by 胡旭林 on 2024/1/20.
//

#ifndef AOS_THREAD_H
#define AOS_THREAD_H

#include "mmu.h"
#include "irq.h"
#include "vm.h"
#include "i386.h"
#include "string.h"
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

struct thread_state{
    uint32_t* thread_stack;
    enum task_status status;
    uint8_t priority;
    uint8_t ticks;
    uint32_t elapsed_ticks;
    ele_t general_tag;
    ele_t blcok_tag;
    ele_t all_list_tag;
    uint32_t* pgdir;
    char name[16];
    uint32_t stack_magic;
};
typedef struct thread_state thread_t;

thread_t* running_thread();
void create_thread(thread_t* pthread, thread_func func, void* args);
void init_thread(thread_t *pthread, char *name, int prio);
void kernel_thread(thread_func* func, void *args);
thread_t* start_thread(char *name, int prio, thread_func func, void *args);
void schedule();
void enable_thread();

void acquiresleep(sleeplock_t *lk);
void releasesleep(sleeplock_t *lk);
void sleep(sleeplock_t *lk, enum task_status status);
void wakeup(sleeplock_t *lk);

#endif //AOS_THREAD_H
