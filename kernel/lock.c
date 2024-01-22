//
// Created by 胡旭林 on 2024/1/22.
//

#include "include/lock.h"

void init_lock(struct spinlock *lk, char *name) {
    strcpy(name, lk->name, strlen(name));
    lk->locked = 0;
}

void acquire(struct spinlock *lk) {
    while (xchg(&lk->locked, 1));
}

void release(struct spinlock *lk) {
    xchg(&lk->locked, 0);
}

void init_sleeplock(struct sleeplock *lk, char *name) {
    init_lock(&lk->lock, "sleeplock");
    strcpy(name, lk->name, strlen(name));
    lk->locked = 0;
    lk->pthread = NULL;
}

void acquiresleep(struct sleeplock *lk) {
    acquire(&lk->lock);
    while (lk->locked) {
        // sleep here. To be implemented
        // remember release lk->lock in sleep()
    }
    lk->locked = 1;
    lk->pthread = running_thread();
    release(&lk->lock);
}

void releasesleep(struct sleeplock *lk) {
    acquire(&lk->lock);
    // wake up here.
    lk->locked = 0;
    lk->pthread = NULL;
    release(&lk->lock);
}