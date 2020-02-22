#include "../include/measure.h"

// Tests
void cycle_time() {
    uint64_t times[10];
    double sum = 0;

    for (int i = 0; i < 10; i++) {
        MEASURE_START();
        sleep(10);
        MEASURE_END();
        times[i] = measure_time();
        sum += times[i];
        printf("cycles: %lu\n", times[i]);
    }

    printf("mean: %lf\n", sum / 10);
}

void reading_overhead() {
    uint64_t times[LOOPS];
    double sum = 0;

    for (int i = 0; i < LOOPS; i++) {
        MEASURE_START();
        MEASURE_END();
        times[i] = measure_time();
        sum += times[i];
    }

    printf("average overhead: %lf\n", sum / LOOPS);
}

void loop_overhead() {
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        for (int i = 0; i < LOOPS; i++) {
            //do nothing
        }
    }
    MEASURE_END();

    printf("average loop overhead: %lf\n", (double)measure_time() / LOOPS);
}

int main() {

    set_affinity();

	// cycle_time();
    reading_overhead();
    loop_overhead();

	return 0;
}