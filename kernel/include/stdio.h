//
// Created by huuxlion on 2023/7/3.
//

#ifndef AOS_STDIO_H
#define AOS_STDIO_H

#include "types.h"

#include <stdarg.h>

uint32_t vsprintf(const char *fmt, char *buf, va_list ap);
uint32_t printf(const char* fmt, ...);
uint32_t printk(const char* fmt, ...);
void sprintf(char *buf, const char *fmt, ...);

#endif //AOS_STDIO_H
