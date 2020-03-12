#include "../include/measure.h"
#include "../include/analysis.h"
#include <fcntl.h>

#define K 1024
#define M (1024 * K)
#define BLOCK (4 * K)

void file_sequential_read(char* file_name, size_t file_size) {
    int fd = open(file_name, O_RDONLY | O_DIRECT);
    if (fd == -1) {
        printf("open failure\n");
        exit(1);
    }
    void* buf = malloc(BLOCK);
    if (buf == NULL) {
        printf("malloc failure");
        exit(1);
    }
    size_t times = file_size / BLOCK;
    // printf("times: %lu\n", times);

    double total = 0;
    for (int i = 0; i < times; i++) {
        MEASURE_START();
        read(fd, buf, BLOCK);
        MEASURE_END();
        total += cycleToS(measure_time());
    }

    close(fd);
    free(buf);
    printf("<seq read> file size: %luMB, total time: %lfms\n", file_size / M, total * 1000);
}

void file_random_read(char* file_name, size_t file_size) {
    int fd = open(file_name, O_RDONLY | O_DIRECT);
    if (fd == -1) {
        printf("open failure\n");
        exit(1);
    }
    void* buf = malloc(BLOCK);
    if (buf == NULL) {
        printf("malloc failure");
        exit(1);
    }
    size_t times = file_size / BLOCK;

    double total = 0;
    for (int i = 0; i < times; i++) {
        int offset = rand() % times;
        lseek(fd, offset * BLOCK, SEEK_SET);
        MEASURE_START();
        read(fd, buf, BLOCK);
        MEASURE_END();
        total += cycleToS(measure_time());
    }

    close(fd);
    free(buf);
    printf("<random read> file size: %luMB, total time: %lfms\n", file_size / M, total * 1000);
}

int main() {
    set_affinity();
    uint64_t file_size = 100 * M;
    char* file_name[] = {"1.dat", "2.dat", "3.dat", "4.dat", "5.dat", "6.dat", "7.dat", "8.dat", "9.dat", "10.dat"};
    for (int i = 0; i < 8; i++) {
        file_sequential_read(file_name[i], file_size);
        file_size *= 2;
    }

    file_size = 100 * M;
    for (int i = 0; i < 8; i++) {
        file_random_read(file_name[i], file_size);
        file_size *= 2;
    }
}