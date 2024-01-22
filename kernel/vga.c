
#include "include/vga.h"

static void update_cursor(size_t x, size_t y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

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
    //asm volatile("cli");
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
    update_cursor(terminal_column, terminal_row);
    //asm volatile("sti");
}

void terminal_clear() {
    for (size_t col = 0; col < VGA_WIDTH; col++) {
        for (size_t row = 0; row < VGA_HEIGHT; row++) {
            size_t index = row * VGA_WIDTH + col;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char *ch) {
    size_t len = strlen(ch);
    for (size_t i = 0; i < len; i++) {
        terminal_putchar(ch[i]);
    }
}

void terminal_writeln(const char* ch) {
    size_t len = strlen(ch);
    for (size_t i = 0; i < len; i++) {
        terminal_putchar(ch[i]);
    }
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        for (size_t line = 1; line < VGA_HEIGHT; line++) {
            terminal_scroll(line);
        }
        terminal_delete_last_line();
        terminal_row = VGA_HEIGHT - 1;
    }
    update_cursor(terminal_column, terminal_row);
}

void terminal_scroll(size_t line) {
    uint16_t* ptr;

    if (line == 0) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            size_t index = x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
        return;
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        size_t index = line * VGA_WIDTH + x;
        terminal_buffer[index - VGA_WIDTH] = terminal_buffer[index];
    }
}

void terminal_delete_last_line() {
    uint16_t* ptr;
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        ptr = VGA_MEMORY + (VGA_HEIGHT - 1) * (VGA_WIDTH * 2) + x * 2;
        *ptr = vga_entry(' ', terminal_color);
    }
}

void terminal_backspace() {
    if (terminal_row == 0 && terminal_column == 0) {
        terminal_put_entry_at(' ', terminal_color, 0, 0);
        return ;
    }
    if (terminal_column == 0) {
        terminal_column = VGA_WIDTH - 1;
        terminal_row--;
        terminal_put_entry_at(' ', terminal_color, terminal_column, terminal_row);
        update_cursor(terminal_column, terminal_row);
        return ;
    }
    terminal_put_entry_at(' ', terminal_color, --terminal_column, terminal_row);
    update_cursor(terminal_column, terminal_row);
}