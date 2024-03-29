//
// Created by huuxlion on 2024/1/22.
//

#ifndef AOS_LOCK_H
#define AOS_LOCK_H

#include "i386.h"
#include "types.h"
#include "string.h"
#include "list.h"

// definition of spinlock
struct spinlock {
    uint locked;
    char name[16];
};

// definition of sleeplock, which contains a spinklock for the mutex access of uint locked
// member chain contains the blocked thread tags
struct sleeplock {
    uint locked;
    struct spinlock lock;
    list_t chain;
    char name[16];
};
typedef struct sleeplock sleeplock_t;

void init_lock(struct spinlock *lk, char *name);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);

void init_sleeplock(struct sleeplock *lk, char *name);

#endif //AOS_LOCK_H
