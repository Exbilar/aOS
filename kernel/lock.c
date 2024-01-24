//
// Created by huuxlion on 2024/1/22.
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
    list_init(&lk->chain);
    strcpy(name, lk->name, strlen(name));
    lk->locked = 0;
}