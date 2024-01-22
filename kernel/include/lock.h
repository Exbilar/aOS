//
// Created by 胡旭林 on 2024/1/22.
//

#ifndef AOS_LOCK_H
#define AOS_LOCK_H

#include "i386.h"
#include "types.h"
#include "string.h"
#include "thread.h"

struct spinlock {
    uint locked;
    char name[16];
};

struct sleeplock {
    uint locked;
    struct spinlock lock;
    thread_t *pthread;
    char name[16];
};

void init_lock(struct spinlock *lk, char *name);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);

void init_sleeplock(struct sleeplock *lk, char *name);

#endif //AOS_LOCK_H
