//
// Created by huuxlion on 2023/7/10.
//

#ifndef AOS_IDT_H
#define AOS_IDT_H

#define NINTS 256

#include "mmu.h"
#include "i386.h"
#include "types.h"

uint32_t set_selector(uint rpl, uint ti, uint index);
uint64_t create_idt_entry(uint32_t off, uint16_t sel, uint d);
void set_idt_entry(uint64_t desc, uint index);
void register_handler(void *handler, uint vec);

void load_idt();
void init_idt();

#endif //AOS_IDT_H
