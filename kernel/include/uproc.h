//
// Created by huxulion on 2024/1/25.
//

#ifndef AOS_UPROC_H
#define AOS_UPROC_H

#include "types.h"
#include "gdt.h"

void create_process(char *name, void *filename_, int prio);
void start_process(void *filename_);

#endif //AOS_UPROC_H
