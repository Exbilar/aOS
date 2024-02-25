//
// Created by huuxlion on 2024/1/20.
//

#include "include/thread.h"
#include "include/gdt.h"
#include "include/irq.h"
#include "include/sleeplock.h"

list_t thread_ready_list;
list_t thread_all_list;
list_t thread_block_list;
thread_t *main_thread;
thread_t *idle_thread;

struct spinlock pid_lock;
struct spinlock list_lock;

extern void switch_to(thread_t *cur_thread, thread_t *next_thread);

static void idle(void *arg);

static pid_t pid_pool = 0;

pid_t alloc_pid() {
    acquire(&pid_lock);
    pid_pool++;
    release(&pid_lock);
    return pid_pool;
}

static void make_main_thread() {
    main_thread = running_thread();
    init_thread(main_thread, "main_thread", 31);
    list_insert_front(&thread_all_list, &main_thread->all_list_tag);
}

void enable_thread() {
    list_init(&thread_ready_list);
    list_init(&thread_all_list);
    list_init(&thread_block_list);
    init_lock(&pid_lock, "pid_lock");
    init_lock(&list_lock, "list_lock");

    make_main_thread();
    idle_thread = start_thread("idle_thread", 10, (thread_func*)idle, NULL);
}

thread_t* running_thread() {
    uint32_t esp;
    asm volatile("movl %%esp, %0" : "=g" (esp));
    return (thread_t*) (esp & 0xfffff000);
}

void init_thread(thread_t *pthread, char *name, int prio) {
    memset(pthread, 0, sizeof (thread_t));
    strcpy(name, pthread->name, strlen(name));
    pthread->priority = prio;
    pthread->ticks = prio;
    pthread->elapsed_ticks = 0;
    pthread->pid = alloc_pid();
    if (pthread == main_thread) {
        pthread->status = TASK_RUNNING;
        uint32_t esp;
        asm volatile("movl %%esp, %0" : "=g" (esp));
        pthread->thread_stack = (uint32_t*) esp;
    }
    else {
        pthread->status = TASK_READY;
        pthread->thread_stack = (uint32_t *) ((uint32_t) pthread + PGSIZE);
    }
    pthread->pgdir = NULL;
    pthread->stack_magic = 0x12345678;
}

void create_thread(thread_t* pthread, thread_func func, void* args) {
    pthread->thread_stack =
            (uint32_t *) ((uint32_t)pthread->thread_stack - sizeof(struct regs));
    pthread->thread_stack =
            (uint32_t *) ((uint32_t)pthread->thread_stack - 4);
    // $4 for popl %eax in intr_exit()
    pthread->thread_stack =
            (uint32_t *) ((uint32_t)pthread->thread_stack - sizeof(struct thread_stack));
    struct thread_stack* kstack = (struct thread_stack*) pthread->thread_stack;
    kstack->eip = (uint32_t) kernel_thread;
    kstack->args_addr = (uint32_t) args;
    kstack->func_addr = (uint32_t) func;
    kstack->ebp = kstack->ebx = kstack->edi = kstack->esi = 0;
}

thread_t* start_thread(char *name, int prio, thread_func func, void *args) {
    thread_t* thread = (thread_t*) kalloc();
    init_thread(thread, name, prio);
    create_thread(thread, func, args);

    assert_write(!find_ele(&thread_ready_list, &thread->general_tag), "panic: thread_ready_list");
    list_insert_rear(&thread_ready_list, &thread->general_tag);

    assert_write(!find_ele(&thread_all_list, &thread->all_list_tag), "panic: thread_all_list");
    list_insert_rear(&thread_all_list, &thread->all_list_tag);

    return thread;
}

void kernel_thread(thread_func* func, void *args) {
    sti();
    func(args);
}

void schedule() {
    push_off();

    thread_t *pthread = running_thread();
    uint32_t offset = (uint32_t) &pthread->general_tag - (uint32_t) pthread;

    if (pthread->status == TASK_RUNNING) {
        pthread->status = TASK_READY;
        assert_write(!find_ele(&thread_ready_list, &pthread->general_tag),
                     "panic: schedule find ele");
        list_insert_rear(&thread_ready_list, &pthread->general_tag);
        pthread->ticks = pthread->priority;
    }
    else if (pthread->status == TASK_BLOCKED) {
        assert_write(!find_ele(&thread_block_list, &pthread->general_tag),
                     "panic: schedule find ele");
        list_insert_rear(&thread_block_list, &pthread->general_tag);
        pthread->ticks = pthread->priority;
    }
    else {
        // To be implemented
    }

    if (is_empty(&thread_ready_list)) {
        task_wakeup(idle_thread);
    }
    assert_write(!is_empty(&thread_ready_list), "panic: no ready task");

    ele_t *ele = thread_ready_list.front.next;
    list_remove(ele);

    thread_t *next_thread = (thread_t *) ((uint32_t) ele - offset);
    next_thread->status = TASK_RUNNING;

    activate_process(next_thread);

    pop_off();
    // switch_to() in intr.S
    switch_to(pthread, next_thread);
}

void task_block(enum task_status status) {
    thread_t *pthread = running_thread();
    pthread->status = status;
    schedule();
}

void task_wakeup(thread_t *pthread) {
    push_off();

    list_remove(&pthread->general_tag);
    assert_write(!find_ele(&thread_ready_list, &pthread->general_tag),
                 "panic task_wakeup: find ele");
    list_insert_rear(&thread_ready_list, &pthread->general_tag);
    pthread->status = TASK_READY;
    pthread->ticks = pthread->priority;

    pop_off();
}

void thread_yield() {
    schedule();
}

static void idle(void *arg) {
    while (true) {
        task_block(TASK_BLOCKED);
        asm volatile("sti; hlt" : : : "memory");
    }
}

extern struct taskstate tss;

void activate_process(thread_t *pthread) {
    assert_write(pthread != NULL, "panic: activate_process");
    uint32_t pa = (uint32_t) PDT_START;
    if (pthread->pgdir != NULL) {            // user process
        pa = (uint32_t) pthread->pgdir;
        tss.esp0 = (uint32_t) ((uint32_t)pthread + PGSIZE);
    }
    lcr3((uint32_t)pa);
}