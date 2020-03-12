#include "../include/measure.h"
#include "../include/analysis.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define NUM_OF_EXPERIMENTS 10
#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

void page_fault() {
    int fd = open("ram/trash.dat", O_CREAT | O_RDWR);
    if (fd == -1) {
        printf("fail to open file\n");
        exit(1);
    }

    const size_t offset = 1 * MB;
    const size_t file_size = 1 * GB;
    const size_t page_num = file_size / offset;

    for (int k = 0; k < NUM_OF_EXPERIMENTS; k++) {
        char* addr = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) {
            printf("mmap failure\n");
            exit(1);
        }

        int idx[page_num];

        for (int i = 0; i < page_num; i++) {
            idx[i] = rand() % page_num;
        }

        MEASURE_START();
        volatile char des;
        for (int i = 0; i < page_num; i++) {
            des = addr[idx[i] * offset];
        }
        MEASURE_END();

        uint64_t cycles = measure_time();
        printf("average page fault overhead: %lf ms\n", cycleToS(cycles) * 1000 / page_num);
        munmap(addr, file_size);
    }

    close(fd);
}

int main() {
    set_affinity();
    // page_fault();

    double time[NUM_OF_EXPERIMENTS] = {0.171953, 0.169190, 0.168684, 0.169082, 0.168624, 0.170484, 0.171265, 0.169604, 0.169657, 0.170029};
    double mean = getMean(time, NUM_OF_EXPERIMENTS);
    double stddev = getStdDev(time, NUM_OF_EXPERIMENTS);
    printf("mean: %lf, stddev: %lf\n", mean, stddev);
}