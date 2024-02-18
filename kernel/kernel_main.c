#include "include/system.h"

struct sleeplock lk;

int cnt = 0;

extern struct ide_channel channels[2];
struct disk *hd = &channels[0].devices[0];

char buf[512];
char empty_buf[512];

struct semaphore sem;

void k_thread_a(void *args) {
    int lba = 1;
    while (1) {
        ide_write(hd, lba, buf, 1);
        lba++;
        timer_wait(100);
    }
    for (;;);
}

void k_thread_b(void *args) {
    int lba = 1;
    while (1) {
        ide_read(hd, lba, empty_buf, 1);
        terminal_write(empty_buf);
        lba++;
        timer_wait(100);
    }
}

void u_process(void *args) {
    int x = 10;
    int y = 0x3f;
    char str[10] = "asldk";
    printf("hello: %d %x: g %s", x, y, str);
    while (1);
}

void kernel_main(uint32_t magic, uint32_t addr) {
    init_all(addr);

    create_process("a", u_process, 10);
    create_process("a", u_process, 10);

    for (;;);
}
