#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "include/vga.h"
#include "include/multiboot.h"
#include "include/stdio.h"
#include "include/gdt.h"

void kernel_main(uint32_t magic, uint32_t addr) {

    init_gdt();

    terminal_init();

    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("hello world!");
    }

    return ;
}
