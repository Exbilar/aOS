#include "include/system.h"

struct sleeplock lk;

void k_thread_a(void *args) {
    char *str = args;
    terminal_write(str);
    for (;;);
}

void kernel_main(uint32_t magic, uint32_t addr) {
    init_all(addr);

    start_thread("I am thread a", 5, (thread_func*)k_thread_a, "argA ");
    start_thread("I am thread b", 5, (thread_func*)k_thread_a, "argB ");
    start_thread("I am thread c", 5, (thread_func*)k_thread_a, "argC ");

    init_sleeplock(&lk, "lock");

    for (;;);
}
