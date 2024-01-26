#include "include/system.h"

struct sleeplock lk;

void k_thread_a(void *args) {
    while (1) {
        terminal_write("a");
        timer_wait(100);
    }
}

void u_process(void *args) {
    int cnt = 0;
    while (1) {
        cnt += 1;
        timer_wait(100);
    }
}

void kernel_main(uint32_t magic, uint32_t addr) {
    init_all(addr);

    start_thread("I am thread a", 5, (thread_func*)k_thread_a, "argA ");
    create_process("process a", u_process, 5);

    for (;;);
}
