//
// Created by 胡旭林 on 2024/1/21.
//

#include "include/list.h"

void list_init(list_t *list) {
    list->front.next = &list->rear;
    list->front.prev = &list->rear;
    list->rear.next  = &list->front;
    list->rear.prev  = &list->front;
}

void check_ele(ele_t *ele) {
    assert_write(ele->prev->next == ele && ele->next->prev == ele, "panic: check_ele");
}

bool is_empty(list_t *list) {
    check_ele(&list->front);
    check_ele(&list->rear);
    return list->front.next == &list->rear && list->rear.prev == &list->front;
}

void insert_after(ele_t *pos, ele_t *ele) {
    ele_t *after = pos->next;
    pos->next = ele;
    ele->prev = pos;
    ele->next = after;
    after->prev = ele;

    check_ele(pos);
    check_ele(ele);
    check_ele(after);
}

void insert_before(ele_t *pos, ele_t *ele) {
    ele_t *before = pos->prev;
    pos->prev = ele;
    ele->next = pos;
    ele->prev = before;
    before->next = ele;

    check_ele(pos);
    check_ele(ele);
    check_ele(before);
}

void remove(ele_t *ele) {
    ele_t *before = ele->prev;
    ele_t *after = ele->next;
    before->next = after;
    after->prev = before;
    ele->prev = ele->next = NULL;
    check_ele(before);
    check_ele(after);
}

void list_insert_front(list_t *list, ele_t *ele) {
    insert_after(&list->front, ele);
}

void list_insert_rear(list_t *list, ele_t *ele) {
    insert_before(&list->rear, ele);
}

bool find_ele(list_t *list, ele_t *ele) {
    for (ele_t *e = list->front.next; e != &list->rear; e = e->next) {
        if (e == ele) return true;
    }
    return false;
}
