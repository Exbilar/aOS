//
// Created by 胡旭林 on 2024/1/17.
//

#ifndef AOS_SYSTEM_H
#define AOS_SYSTEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vga.h"
#include "multiboot.h"
#include "stdio.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "kalloc.h"
#include "vm.h"

void mem_init(uint32_t addr) {
    multiboot_info_t* mbi = (multiboot_info_t*) addr;

    // kalloc start from here, 0 ~ 2MiB memory area is used for kernel
    uint32_t pa_start = 0x300000;
    uint32_t pa_end = 0x100000 + mbi->mem_upper * 1024;
    kinit((void *)pa_start, (void *)pa_end);
    pagetable_init();
}

#endif //AOS_SYSTEM_H
