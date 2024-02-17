//
// Created by huxulion on 2024/2/15.
//

#include "include/thread.h"
#include "include/sleeplock.h"
#include "include/irq.h"

extern list_t thread_ready_list;
extern list_t thread_all_list;
extern list_t thread_block_list;

extern void switch_to(thread_t *cur_thread, thread_t *next_thread);
extern thread_t *idle_thread;

static void sleep(sleeplock_t *lk, enum task_status status) {
    assert_write(status == TASK_WAITING || status == TASK_HANGING || status == TASK_BLOCKED,
                 "panic: sleep status");

    thread_t *pthread = running_thread();
    pthread->status = status;

    if (pthread->status == TASK_BLOCKED) {
        assert_write(!find_ele(&thread_block_list, &pthread->general_tag),
                     "panic: schedule find ele");
        list_insert_rear(&thread_block_list, &pthread->general_tag);
        list_insert_rear(&lk->chain, &pthread->blcok_tag);
        pthread->ticks = pthread->priority;
    }
    else {
        // ...
    }

    uint32_t offset = (uint32_t) &pthread->general_tag - (uint32_t) pthread;

    if (is_empty(&thread_ready_list)) {
        task_wakeup(idle_thread);
    }

    assert_write(!is_empty(&thread_ready_list), "panic: no ready task");

    ele_t *ele = thread_ready_list.front.next;
    list_remove(ele);

    thread_t *next_thread = (thread_t *) ((uint32_t) ele - offset);
    next_thread->status = TASK_RUNNING;

    activate_process(next_thread);

    release(&lk->lock);
    pop_off();

    switch_to(pthread, next_thread);
}

static void wakeup(sleeplock_t *lk) {
    thread_t *pthread = running_thread();
    uint32_t offset = (uint32_t) &pthread->blcok_tag - (uint32_t) pthread;
    for (ele_t *ele = lk->chain.front.next; ele != &lk->chain.rear; ele = ele->next) {
        thread_t *thread = (thread_t *) ((uint32_t) ele - offset);
        thread->status = TASK_READY;
        assert_write(find_ele(&thread_block_list, &thread->general_tag),
                     "panic: wakeup not find ele");
        list_remove(&thread->general_tag);
        list_insert_rear(&thread_ready_list, &thread->general_tag);
    }
    list_init(&lk->chain);
}

void acquiresleep(sleeplock_t *lk) {
    acquire(&lk->lock);
    while (lk->locked) {
        sleep(lk, TASK_BLOCKED);
        acquire(&lk->lock);   //  Must acquire the spinlock again after waking up !!!
    }
    lk->locked = 1;
    release(&lk->lock);
}

void releasesleep(sleeplock_t *lk) {
    acquire(&lk->lock);
    lk->locked = 0;
    wakeup(lk);
    release(&lk->lock);
}

void cond_wait(sleeplock_t *lk) {
    acquire(&lk->lock);
    sleep(lk, TASK_BLOCKED);
}

void cond_signal(sleeplock_t *lk) {
    acquire(&lk->lock);
    wakeup(lk);
    release(&lk->lock);
}

void init_semaphore(struct semaphore *sem, int val) {
    sem->val = val;
    sem->wakeup = 0;
    init_sleeplock(&sem->sleep_lock, "sem_lock");
}

void semaphore_P(struct semaphore *sem) {
    push_off();
    acquire(&sem->sleep_lock.lock);
    sem->val--;
    if (sem->val < 0) {
        do {
            sleep(&sem->sleep_lock, TASK_BLOCKED);      // pop_off() here
            push_off();
            acquire(&sem->sleep_lock.lock);
        } while (sem->wakeup == 0);
        sem->wakeup--;
    }
    release(&sem->sleep_lock.lock);
    pop_off();
}

void semaphore_V(struct semaphore *sem) {
    push_off();
    acquire(&sem->sleep_lock.lock);
    sem->val++;
    if (sem->val <= 0) {
        wakeup(&sem->sleep_lock);
        sem->wakeup++;
    }
    release(&sem->sleep_lock.lock);
    pop_off();
}