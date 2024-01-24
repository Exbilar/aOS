#include "include/system.h"

struct sleeplock lk;

void k_thread_a(void *args) {
    char *str = args;
    acquiresleep(&lk);
    timer_wait(100);
    terminal_write(str);
    timer_wait(10);
    for (;;);
}

void kernel_main(uint32_t magic, uint32_t addr) {
    init_all(addr);

    start_thread("I am thread a", 5, (thread_func*)k_thread_a, "argA ");
    start_thread("I am thread b", 5, (thread_func*)k_thread_a, "argB ");
    start_thread("I am thread c", 5, (thread_func*)k_thread_a, "argC ");

    init_sleeplock(&lk, "lock");

    timer_wait(100);
    while (1) {
        terminal_write("main ");
        timer_wait(10);
        releasesleep(&lk);
    }
}
