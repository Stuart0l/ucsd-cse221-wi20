#include "../include/measure.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

#define NUM_OF_EXPERIMENTS 10
#define LOOPS 1000
#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

void page_fault() {
    int fd = open("ram/file.dat", O_CREAT | O_RDWR);
    if (fd == -1) {
        printf("fail to open file\n");
        exit(1);
    }

    const int offset = 1 * MB;
    const int file_size = 1 * GB;
    printf("file size: %d\n", file_size);
    const int page_num = file_size / offset;
    char* addr = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        printf("mmap failure\n");
        exit(1);
    }

    for (int k = 0; k < NUM_OF_EXPERIMENTS; k++) {
        MEASURE_START();
        char des;
        for (int i = 0; i < page_num; i++) {
            des = addr[i * offset];
        }
        MEASURE_END();

        uint64_t cycles = measure_time();
        printf("average page fault overhead: %lfcycles\n", (double)cycles / page_num);
    }

    munmap(addr, file_size);
    close(fd);
}

int main() {
    page_fault();
}