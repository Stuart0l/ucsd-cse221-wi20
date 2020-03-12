#include "../include/measure.h"
#include "../include/analysis.h"
#include <stdlib.h>
#include <string.h>

#define NUM_OF_EXPERIMENTS 10
#define KB 1024 
#define MB 1024 * KB
#define GB 1024 * MB

void read_bandwidth() {
    const size_t src_size = 1 * GB;
    const size_t des_size = 4 * MB;
    char* src = (char*) malloc(src_size);   // 100mb data should be sufficient to defeat any cache
    char* des = (char*) malloc(des_size);   // 4mb array can fit into cache, which reduce the writing overhead
    if (src == NULL || des == NULL) {
        printf("malloc failure\n");
        exit(1);
    }

    // warm up the cache
    #pragma unroll
    for (int j = 0; j < src_size / des_size; j++) {
        memcpy(des, (const void *)&src[j * des_size], des_size);
    }

    double results[NUM_OF_EXPERIMENTS];
    for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {
        MEASURE_START();

        #pragma unroll
        for (int j = 0; j < src_size / des_size; j++) {
            memcpy(des, (const void *)&src[j * des_size], des_size);
        }

        MEASURE_END();

        double seconds = (double) measure_time() / (2.4 * GB);   // cpu freq is 2.4GHz
        results[i] = (double)1 / seconds;
        printf("reading bandwidth: %lfGB/s\n", results[i]);
    }

    double mean = getMean(results, NUM_OF_EXPERIMENTS);
    double stddev = getStdDev(results, NUM_OF_EXPERIMENTS);
    printf("mean: %lf, stddev: %lf\n", mean, stddev);

    free(src);
    free(des);
}

void write_bandwidth() {
    const size_t src_size = 4 * MB;
    const size_t des_size = 1 * GB;
    char* src = (char*) malloc(src_size);
    char* des = (char*) malloc(des_size);
    if (src == NULL || des == NULL) {
        printf("malloc failure\n");
        exit(1);
    }

    // warm up the cache
    #pragma unroll
    for (int j = 0; j < des_size / src_size; j++) {
        memcpy((void *)&des[j * src_size], src, src_size);
    }

    double results[NUM_OF_EXPERIMENTS];
    for (int i = 0; i < NUM_OF_EXPERIMENTS; i++) {
        MEASURE_START();

        #pragma unroll
        for (int j = 0; j < des_size / src_size; j++) {
            memcpy((void *)&des[j * src_size], src, src_size);
        }

        MEASURE_END();

        double seconds = (double) measure_time() / (2.4 * GB);
        double write_bandwidth = (double)1 / seconds;
        printf("writing bandwidth: %lfGB/s\n", write_bandwidth);
        results[i] = write_bandwidth;
    }

    double mean = getMean(results, NUM_OF_EXPERIMENTS);
    double stddev = getStdDev(results, NUM_OF_EXPERIMENTS);
    printf("mean: %lf, stddev: %lf\n", mean, stddev);

    free(src);
    free(des);
}

int main() {
    set_affinity();
    read_bandwidth();
    write_bandwidth();
}