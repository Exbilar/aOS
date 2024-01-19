#include "include/system.h"

void kernel_main(uint32_t magic, uint32_t addr) {
    terminal_init();
    init_gdt();
    init_intr();
    mem_init(addr);

    for (;;);
}
