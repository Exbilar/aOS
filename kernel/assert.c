//
// Created by 胡旭林 on 2023/7/3.
//

#include "include/assert.h"

void assert(bool expr) {
    if (expr == false) {
        asm volatile("hlt");
    }
}
