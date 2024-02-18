//
// Created by huuxlion on 2024/1/22.
//

#include "include/kbd.h"

#define KBD_BUF_PORT 0x60

#define esc '\033'
#define backspace '\b'
#define tab '\t'
#define enter '\r'
#define delete '\177'

#define char_invisible 0
#define left_ctrl char_invisible
#define right_ctrl char_invisible
#define left_shift char_invisible
#define right_shift char_invisible
#define left_alt char_invisible
#define right_alt char_invisible
#define caps_lock char_invisible

#define l_shift_makecode 0x2a
#define r_shift_makecode 0x36
#define l_alt_makecode 0x38
#define r_alt_makecode 0xe038
#define r_alt_breakcode 0xe0b8
#define l_ctrl_makecode 0x1d
#define r_ctrl_makecode 0xe01d
#define r_ctrl_breakcode 0xe09d
#define caps_lock_makecode 0x3a

// scan code keymap: keymap[code][shift] = key
static char keymap[][2] = {{0, 0},
                           {esc, esc},
                           {'1', '!'},
                           {'2', '@'},
                           {'3', '#'},
                           {'4', '$'},
                           {'5', '%'},
                           {'6', '^'},
                           {'7', '&'},
                           {'8', '*'},
                           {'9', '('},
                           {'0', ')'},
                           {'-', '_'},
                           {'=', '+'},
                           {backspace, backspace},
                           {tab, tab},
                           {'q', 'Q'},
                           {'w', 'W'},
                           {'e', 'E'},
                           {'r', 'R'},
                           {'t', 'T'},
                           {'y', 'Y'},
                           {'u', 'U'},
                           {'i', 'I'},
                           {'o', 'O'},
                           {'p', 'P'},
                           {'[', '{'},
                           {']', '}'},
                           {enter, enter},
                           {left_ctrl, left_ctrl},
                           {'a', 'A'},
                           {'s', 'S'},
                           {'d', 'D'},
                           {'f', 'F'},
                           {'g', 'G'},
                           {'h', 'H'},
                           {'j', 'J'},
                           {'k', 'K'},
                           {'l', 'L'},
                           {';', ':'},
                           {',', '"'},
                           {'`', '~'},
                           {left_shift, left_shift},
                           {'\\', '|'},
                           {'z', 'Z'},
                           {'x', 'X'},
                           {'c', 'C'},
                           {'v', 'V'},
                           {'b', 'B'},
                           {'n', 'N'},
                           {'m', 'M'},
                           {',', '<'},
                           {'.', '>'},
                           {'/', '?'},
                           {right_shift, right_shift},
                           {'*', '*'},
                           {left_alt, left_alt},
                           {' ', ' '},
                           {caps_lock, caps_lock}};

static bool ctrl_status, shift_status, alt_status, caps_lock_status, ext_scancode;

void kbd_handler(struct regs *r) {
    uint16_t scancode = inb(KBD_BUF_PORT);
    bool breakcode = ((scancode & 0x0080) != 0);

    if (scancode == 0xe0) {
        ext_scancode = true;
        return ;
    }

    if (ext_scancode) {
        scancode = ((0xe000) | (scancode));
        ext_scancode = false;
    }

    if (breakcode) {
        uint16_t makecode = (scancode & 0xff7f);
        if (makecode == l_ctrl_makecode || makecode == r_ctrl_makecode) {
            ctrl_status = false;
        }
        else if (makecode == l_alt_makecode || makecode == r_alt_makecode) {
            alt_status = false;
        }
        else if (makecode == l_shift_makecode || makecode == r_shift_makecode) {
            shift_status = false;
        }
        return ;
    }

    if (scancode == l_shift_makecode || scancode == r_shift_makecode) {
        shift_status = true;
        return ;
    }

    if (scancode == l_ctrl_makecode || scancode == r_ctrl_makecode) {
        ctrl_status = true;
        return ;
    }

    if (scancode == l_alt_makecode || scancode == r_alt_makecode) {
        alt_status = true;
        return ;
    }

    if (scancode == caps_lock_makecode) {
        caps_lock_status = !caps_lock_status;
        return ;
    }

    if ((scancode < 0 || scancode > 58) && (scancode != r_ctrl_makecode) && (scancode != r_alt_makecode)) {
        terminal_write("unknown character");
        return ;
    }

    int shift = 0;
    if (shift_status || caps_lock_status) {
        shift = 1;
    }

    size_t index = (scancode & 0x00ff);
    char ch = keymap[index][shift];
    if (ch) {
        if (ch == tab) {
            terminal_write(" ");
            return ;
        }
        if (ch == esc) {
            return ;
        }
        if (ch == backspace) {
            terminal_backspace();
            return ;
        }
        if (ch == enter) {
            return ;
        }

        char buf[2];
        buf[0] = ch;
        buf[1] = 0;

        terminal_write(buf);
        return ;
    }
    terminal_write(" #Unknown Character ");
}

void kbd_install() {
    irq_install(0x21, kbd_handler);
}

void kbd_init() {
    kbd_install();
}