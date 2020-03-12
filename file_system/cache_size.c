#include "../include/measure.h"
#include <fcntl.h>
#include "../include/analysis.h"

#define K 1024
#define M (1024 * K)
#define BLOCK (4 * K)

void file_cache_size(char* file_name, size_t file_size) {
    int fd = open(file_name, O_RDONLY);
    lseek(fd, file_size - 1, SEEK_CUR);     // read the file in the reversed order
    size_t times = file_size / BLOCK;
    void* buf = malloc(BLOCK);

    // warm up the file cache
    for (int i = 0; i < times; i++) {
        lseek(fd, -2 * BLOCK, SEEK_CUR);
        read(fd, buf, BLOCK);
    }
    close(fd);

    fd = open(file_name, O_RDONLY);
    lseek(fd, file_size - 1, SEEK_CUR);

    uint64_t total = 0;
    for (int i = 0; i < times; i++) {
        lseek(fd, -2 * BLOCK, SEEK_CUR);
        MEASURE_START();
        read(fd, buf, BLOCK);
        MEASURE_END();
        total += measure_time();
    }

    close(fd);
    free(buf);
    printf("file size: %lu MB, total access time: %lf ms, average block access time: %lf µs\n", file_size / M, cycleToS(total) * 1000, cycleToS(total) * 1000000 / times);
}

int main() {
    set_affinity();
    uint64_t file_size = 100 * M;
    char* file_name[] = {"1.dat", "2.dat", "3.dat", "4.dat", "5.dat", "6.dat", "7.dat", "8.dat", "9.dat", "10.dat"};
    for (int i = 0; i < 10; i++) {
        file_cache_size(file_name[i], file_size);
        file_size *= 2;
    }
}