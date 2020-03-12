#include "../include/measure.h"
#include "../include/analysis.h"
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

#define K 1024
#define M (1024 * K)
#define BLOCK (4 * K)
#define MAXPROCS 10
#define FILE_SIZE (1000 * M)

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
    size_t times = 100; //file_size / BLOCK;

    double total = 0; // ms
    for (int i = 0; i < times; i++) {
        int offset = rand() % times;
        lseek(fd, offset * BLOCK, SEEK_SET);
        MEASURE_START();
        read(fd, buf, BLOCK);
        MEASURE_END();
        total += cycleToS(measure_time()) * 1000;
    }

    close(fd);
    free(buf);
    printf("<random read> file size: %luMB, total time: %lfms, average block reading time: %lfms\n", file_size / M, total, total / times);
}

void read_contention() {
    char* file_name[] = {"1.dat", "2.dat", "3.dat", "4.dat", "5.dat", "6.dat", "7.dat", "8.dat", "9.dat", "10.dat"};
    for (int i = 1; i <= MAXPROCS; i++) {
        printf("Test for %d processes\n", i);
        for (int j = 0; j < i; j++) {
            pid_t pid = fork();
            if (pid < 0) {
                printf("fork failure\n");
                exit(1);
            }
            if (pid == 0) {
                file_random_read(file_name[j], FILE_SIZE);
                exit(0);
            }
        }
        
        for (int j = 0; j < i; j++) wait(NULL);
    }
}

int main() {
    set_affinity();
    srand(time(NULL));
    read_contention();
}