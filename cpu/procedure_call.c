#include "../include/measure.h"

void f0() {}
void f1(int a) {}
void f2(int a, int b) {}
void f3(int a, int b, int c) {}
void f4(int a, int b, int c, int d) {}
void f5(int a, int b, int c, int d, int e) {}
void f6(int a, int b, int c, int d, int e, int f) {}
void f7(int a, int b, int c, int d, int e, int f, int g) {}

void procedure_overhead() {
    unsigned long long sum = 0;

   for (int i = 0; i < LOOPS; i++) {
        f1(i);f1(i);f1(i);f1(i);f1(i);
        f1(i);f1(i);f1(i);f1(i);f1(i);
    }

    //zero parameter test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f0();f0();f0();f0();f0();
        f0();f0();f0();f0();f0();
    }
    MEASURE_END();
    printf("average procedure call overhead (zero parameter): %lf\n", (double)measure_time() / LOOPS);

    //one parameter test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f1(i);f1(i);f1(i);f1(i);f1(i);
        f1(i);f1(i);f1(i);f1(i);f1(i);
    }
    MEASURE_END();
    printf("average procedure call overhead (one parameter): %lf\n", (double)measure_time() / LOOPS);

    //two parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f2(i, i);f2(i, i);f2(i, i);f2(i, i);f2(i, i);
        f2(i, i);f2(i, i);f2(i, i);f2(i, i);f2(i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (two parameters): %lf\n", (double)measure_time() / LOOPS);

    //three parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f3(i, i, i);f3(i, i, i);f3(i, i, i);f3(i, i, i);f3(i, i, i);
        f3(i, i, i);f3(i, i, i);f3(i, i, i);f3(i, i, i);f3(i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (three parameters): %lf\n", (double)measure_time() / LOOPS);

    //four parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f4(i, i, i, i);f4(i, i, i, i);f4(i, i, i, i);f4(i, i, i, i);f4(i, i, i, i);
        f4(i, i, i, i);f4(i, i, i, i);f4(i, i, i, i);f4(i, i, i, i);f4(i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (four parameters): %lf\n", (double)measure_time() / LOOPS);

    //five parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
        f5(i, i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (five parameters): %lf\n", (double)measure_time() / LOOPS);

    //six parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
        f6(i, i, i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (six parameters): %lf\n", (double)measure_time() / LOOPS);

    //seven parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
        f7(i, i, i, i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (seven parameters): %lf\n", (double)measure_time() / LOOPS);

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

    unsigned long sum = 0;    
    for (int i = 0; i < LOOPS; i++) {
        MEASURE_START();
        for (int i = 0; i < LOOPS; i++) {
            //do nothing
        }
        MEASURE_END();
        sum += measure_time();
    }
    

    printf("average loop overhead: %lf\n", (double)sum / LOOPS);
}

int main() {

    set_affinity();

	procedure_overhead();
    reading_overhead();
    loop_overhead();

	return 0;
}