//
// Created by huxulion on 2024/2/15.
//

#ifndef AOS_SLEEPLOCK_H
#define AOS_SLEEPLOCK_H

#include "lock.h"

void acquiresleep(sleeplock_t *lk);
void releasesleep(sleeplock_t *lk);

void cond_wait(sleeplock_t *lk);
void cond_signal(sleeplock_t *lk);

struct semaphore {
    struct sleeplock sleep_lock;
    int wakeup;
    int val;
};

void semaphore_P(struct semaphore *sem);
void semaphore_V(struct semaphore *sem);
void init_semaphore(struct semaphore *sem, int val);

#endif //AOS_SLEEPLOCK_H
