//
// Created by huuxlion on 2023/7/3.
//

#include "include/stdio.h"
#include "include/vga.h"
#include "include/syscall.h"

static void itoa(uint32_t val, char **buf_addr_ptr, uint8_t base) {
    uint32_t m = val % base;
    uint32_t i = val / base;
    if (i) {
        itoa(i, buf_addr_ptr, base);
    }
    if (m < 10) {
        *((*buf_addr_ptr)++) = (char) (m + '0');
    }
    else {
        *((*buf_addr_ptr)++) = (char) (m + 'a' - 10);
    }
}

uint32_t vsprintf(const char *fmt, char *buf, va_list ap) {
    char *buf_ptr = buf;
    const char *fmt_ptr = fmt;

    int arg_int;
    char* arg_str;

    while (*fmt_ptr) {
        if (*fmt_ptr != '%') {
            *(buf_ptr++) = *(fmt_ptr++);
            continue;
        }
        fmt_ptr++;
        switch (*fmt_ptr++) {
            case 'x':
                arg_int = va_arg(ap, int);
                *(buf_ptr++) = '0';
                *(buf_ptr++) = 'x';
                itoa(arg_int, &buf_ptr, 16);
                break;
            case 's':
                arg_str = va_arg(ap, char*);
                strcpy(arg_str, buf_ptr, strlen(arg_str));
                buf_ptr += strlen(arg_str);
                break;
            case 'd':
                arg_int = va_arg(ap, int);
                itoa(arg_int, &buf_ptr, 10);
                break;
            case 'c':
                arg_int = va_arg(ap, int);
                *(buf_ptr++) = (char) arg_int;
                break;
            default:
                break;
        }
    }

    return strlen(buf);
}

void sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf(fmt, buf, args);
    va_end(args);
}

// used for kernel
uint32_t printk(const char *fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    uint32_t res = vsprintf(fmt, buf, args);
    va_end(args);
    terminal_write(buf);
    return res;
}

// used for user process
uint32_t printf(const char *fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsprintf(fmt, buf, args);
    va_end(args);
    return write(buf);
}