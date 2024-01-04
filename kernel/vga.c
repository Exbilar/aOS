
#include "include/vga.h"

void terminal_init(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_put_entry_at(unsigned char uc, uint8_t color, size_t x, size_t y) {
    size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(uc, color);
}

void terminal_putchar(char c) {
    c = (unsigned char) c;
    terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            for (size_t line = 1; line < VGA_HEIGHT; line++) {
                terminal_scroll(line);
            }
            terminal_delete_last_line();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_write(const char* ch) {
    size_t len = strlen(ch);
    for (size_t i = 0; i < len; i++) {
        terminal_putchar(ch[i]);
    }
    terminal_row += 1;
    terminal_column = 0;
}

void terminal_scroll(size_t line) {
    uint16_t* ptr;

    if (line == 0) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            ptr = VGA_MEMORY + x * 2;
            *ptr = 0;
        }
        return;
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        ptr = VGA_MEMORY + line * (VGA_WIDTH * 2) + x * 2;
        *(ptr - VGA_WIDTH * 2) = *ptr;
    }
}

void terminal_delete_last_line() {
    uint16_t* ptr;
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        ptr = VGA_MEMORY + (VGA_HEIGHT - 1) * (VGA_WIDTH * 2) + x * 2;
        *ptr = 0;
    }
}