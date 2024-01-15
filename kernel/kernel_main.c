#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "include/vga.h"
#include "include/multiboot.h"
#include "include/stdio.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/irq.h"

void kernel_main(uint32_t magic, uint32_t addr) {
    terminal_init();
    init_gdt();
    init_intr();
    for (;;);
}
