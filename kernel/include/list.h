//
// Created by 胡旭林 on 2024/1/21.
//

#ifndef AOS_LIST_H
#define AOS_LIST_H

#include "string.h"
#include "assert.h"

struct list_ele {
    struct list_ele *next;
    struct list_ele *prev;
};
typedef struct list_ele ele_t;

struct list {
    struct list_ele front;
    struct list_ele rear;
};
typedef struct list list_t;

void list_init(list_t *list);
void check_ele(ele_t *ele);
bool is_empty(list_t *list);
void insert_after(ele_t *pos, ele_t *ele);
void insert_before(ele_t *pos, ele_t *ele);
void remove(ele_t *ele);
void list_insert_front(list_t *list, ele_t *ele);
void list_insert_rear(list_t *list, ele_t *ele);
bool find_ele(list_t *list, ele_t *ele);

#endif //AOS_LIST_H
