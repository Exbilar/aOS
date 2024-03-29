//
// Created by huuxlion on 2023/7/3.
//

#include "include/assert.h"

void panic(char* ch) {
    terminal_writeln(ch);
    assert(false);
}

void assert(bool expr) {
    if (expr == false) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}

void assert_write(bool expr, char *ch) {
    if (expr == false) {
        terminal_writeln(ch);
        assert(false);
    }
}