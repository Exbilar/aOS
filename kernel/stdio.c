//
// Created by 胡旭林 on 2023/7/3.
//

#include "include/stdio.h"

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>

void panic(char* ch) {
    terminal_write(ch);
    assert(false);
}

int printf(const char* format, ...) {
    return 0;
}