#include "../include/measure.h"
#include <stdlib.h>
#include <string.h>

#define NUM_OF_EXPERIMENTS 10
#define LOOPS 1000
#define MB 1024 * 1024
#define GB 1024 * MB

void read_bandwidth() {
    const size_t src_size = 128 * MB;
    const size_t des_size = 4 * MB;
    char* src = (char*) malloc(src_size);   // 100mb data should be sufficient to defeat any cache
    char* des = (char*) malloc(des_size);   // 4mb array can fit into cache, which reduce the writing overhead
    if (src == NULL || des == NULL) {
        printf("malloc failure\n");
        exit(1);
    }

    // initialize the src array
    for (int i = 0; i < src_size; i++) {
        src[i] = (char) rand();
    }

    for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {
        MEASURE_START();

        #pragma unroll
        for (int j = 0; j < LOOPS; j++) {
            int offset = rand() % (src_size / des_size) * des_size;
            memcpy(des, (const void *)&src[offset], des_size);
        }

        MEASURE_END();

        double seconds = (double) measure_time() / (2.4 * GB);   // cpu freq is 2.4GHz
        double read_bandwidth = (double)4 * LOOPS / seconds;
        printf("reading bandwidth: %lfMB/s\n", read_bandwidth);
    }

    free(src);
    free(des);
}

void write_bandwidth() {
    const size_t src_size = 4 * MB;
    const size_t des_size = 128 * MB;
    char* src = (char*) malloc(src_size);
    char* des = (char*) malloc(des_size);
    if (src == NULL || des == NULL) {
        printf("malloc failure\n");
        exit(1);
    }

    for (int i = 0; i < src_size; i++) {
        src[i] = (char) rand();
    }

    for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {
        MEASURE_START();

        #pragma unroll
        for (int j = 0; j < LOOPS; j++) {
            int offset = rand() % (des_size / src_size) * src_size;
            memcpy((void *)&des[offset], src, src_size);
        }

        MEASURE_END();

        double seconds = (double) measure_time() / (2.4 * GB);
        double write_bandwidth = (double)4 * LOOPS / seconds;
        printf("writing bandwidth: %lfMB/s\n", write_bandwidth);
    }
}

int main() {
    read_bandwidth();
    write_bandwidth();
}