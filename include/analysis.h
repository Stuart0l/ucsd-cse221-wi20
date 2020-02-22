#include <math.h>
// #include "measure.h"
#include <stdio.h>
#include <stdint.h>

#define WARMUP 100
#define ITERATIONS 1000

double getMean(double* arr, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++)
        sum += arr[i];
    return sum / size;
}

double getStdDev(double* arr, int size) {
    double mean = getMean(arr, size);
    double sum = 0;
    for (int i = 0; i < size; i++)
        sum += (arr[i] - mean) * (arr[i] - mean);
    return sqrt(sum / size);
}

double cycleToS(uint64_t cycles) {
    const long cpuFreq = 2.4 * 1000 * 1000 * 1000;
    return (double) cycles / cpuFreq;
}

// void analyze(void (*test)(), int nums) {
//     double time[nums];
    
//     for (int i = 0; i < WARMUP; i++)
//         test();

//     for (int i = 0; i < nums; i++) {
//         MEASURE_START();
//         for (int j = 0; i < ITERATIONS; i++)
//             test();
//         MEASURE_END();
//         time[i] = cycleToS(measure_time()) / ITERATIONS;
//     }

//     printf("mean: %lf\n", getMean(time, nums));
//     printf("stddev: %lf\n", getStdDev(time, nums));
// }
