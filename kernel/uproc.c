//
// Created by huxulion on 2024/1/25.
//

#include "include/uproc.h"
#include "include/thread.h"
#include "include/irq.h"
#include "include/kalloc.h"

void init_vaddr(struct vaddr *vmem) {
    vmem->mem_start = 0x40000000;   // user space start at 1GB
    vmem->freelist = NULL;
    init_lock(&vmem->lock, "vmem");
}

void init_pgdir(struct vaddr *vmem, uint32_t *pgdir) {
    for (int i = 0; i < 256; i++) {
        pgdir[i] = PDT_START[i];
    }
}

// uproc pagetable enabled
void start_process(void *filename_) {
    thread_t *pthread = running_thread();
    pthread->thread_stack =
            (uint32_t*) ((uint32_t)pthread->thread_stack + sizeof(struct thread_stack) + 4);
    // $4 for popl %eax in intr_exit()
    struct regs *proc_stack = (struct regs*) pthread->thread_stack;
    proc_stack->edi = proc_stack->esi = proc_stack->ebp = proc_stack->esp = 0;
    proc_stack->ebx = proc_stack->edx = proc_stack->ecx = proc_stack->eax = 0;
    proc_stack->gs = 0;
    proc_stack->es = proc_stack->ds = proc_stack->fs = USER_DATA_SEL;
    proc_stack->eip = (uint32_t) filename_;
    proc_stack->cs = USER_CODE_SEL;
    proc_stack->eflags = (FL_IOPL_0 | FL_IF | FL_MBS);
    proc_stack->useresp = pthread->user_vmem.mem_start;
    proc_stack->ss = USER_DATA_SEL;
    asm volatile("movl %0, %%esp; jmp intr_exit" : : "g" ((char *)proc_stack - 4) : "memory");
}

void alloc_first_page(thread_t *thread) {
    uint32_t *firstpage = kalloc();
    mappage(thread->pgdir, thread->user_vmem.mem_start, (uint32_t) firstpage);
    thread->user_vmem.mem_start += PGSIZE;
}

void create_process(char *name, void *filename_, int prio) {
    thread_t *thread = start_thread(name, prio, start_process, filename_);
    thread->pgdir = kalloc();
    init_vaddr(&thread->user_vmem);
    init_pgdir(&thread->user_vmem, thread->pgdir);
    alloc_first_page(thread);
}