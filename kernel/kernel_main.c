#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "include/vga.h"

void kernel_main() {
    terminal_init();
    terminal_write("Hello World!");
    return ;
}
