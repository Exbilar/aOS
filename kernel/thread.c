//
// Created by 胡旭林 on 2024/1/20.
//

#include "include/thread.h"

list_t thread_ready_list;
list_t thread_all_list;
thread_t *main_thread;

extern void switch_to(thread_t *cur_thread, thread_t *next_thread);

static void make_main_thread() {
    main_thread = running_thread();
    init_thread(main_thread, "main_thread", 10);
    list_insert_front(&thread_all_list, &main_thread->all_list_tag);
}

void enable_thread() {
    list_init(&thread_ready_list);
    list_init(&thread_all_list);
    make_main_thread();
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
    pthread->thread_stack -= sizeof(thread_t);
    pthread->thread_stack -= sizeof(struct thread_stack);
    struct thread_stack* kstack = (struct thread_stack*) pthread->thread_stack;
    kstack->eip = (uint32_t) kernel_thread;
    kstack->args_addr = (uint32_t) args;
    kstack->func_addr = (uint32_t) func;
    kstack->ebp = kstack->ebx = kstack->edi = kstack->esi = 0;
}

thread_t* start_thread(char *name, int prio, thread_func func, void *args) {
    thread_t* thread = (thread_t*) malloc_page();
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
    thread_t *pthread = running_thread();

    uint32_t offset = (uint32_t) &pthread->general_tag - (uint32_t) pthread;

    if (pthread->status == TASK_RUNNING) {
        pthread->status = TASK_READY;
        assert_write(!find_ele(&thread_ready_list, &pthread->general_tag),
                     "panic: schedule find ele");
        list_insert_rear(&thread_ready_list, &pthread->general_tag);
        pthread->ticks = pthread->priority;
    }
    else {
        // To be implemented
    }

    assert_write(!is_empty(&thread_ready_list), "panic: no ready task");

    ele_t *ele = thread_ready_list.front.next;
    remove(ele);

    thread_t *next_thread = (thread_t *) ((uint32_t) ele - offset);
    next_thread->status = TASK_RUNNING;

    // switch_to() in intr.S
    switch_to(pthread, next_thread);
}