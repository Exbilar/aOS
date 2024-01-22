//
// Created by 胡旭林 on 2023/7/3.
//

#include "include/assert.h"

void assert(bool expr) {
    if (expr == false) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}

void assert_write(bool expr, char *ch) {
    if (expr == false) {
        assert(false);
        terminal_writeln(ch);
    }
}