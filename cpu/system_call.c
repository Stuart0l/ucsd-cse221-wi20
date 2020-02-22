#include "../include/measure.h"

void syscall_overhead() {
    MEASURE_START();
    getpid();
    MEASURE_END();
    printf("system call (getpid) overhead: %lf\n", (double)measure_time());
}

int main() {
    set_affinity();
    syscall_overhead();
    return 0;
}