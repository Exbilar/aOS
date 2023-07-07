//
// Created by 胡旭林 on 2023/7/3.
//

#ifndef AOS_STDIO_H
#define AOS_STDIO_H

#include "vga.h"
#include "assert.h"

void panic(char* ch);

int printf(const char* format, ...);

#endif //AOS_STDIO_H
