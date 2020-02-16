#include "cpu.h"

void f0() {}
void f1(int a) {}
void f2(int a, int b) {}
void f3(int a, int b, int c) {}
void f4(int a, int b, int c, int d) {}
void f5(int a, int b, int c, int d, int e) {}
void f6(int a, int b, int c, int d, int e, int f) {}
void f7(int a, int b, int c, int d, int e, int f, int g) {}

void procedure_overhead() {

    //zero parameter test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f0();
    }
    MEASURE_END();
    printf("average procedure call overhead (zero parameter): %lf\n", (double)measure_time() / LOOPS);

    //one parameter test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f1(i);
    }
    MEASURE_END();
    printf("average procedure call overhead (one parameter): %lf\n", (double)measure_time() / LOOPS);

    //two parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f2(i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (two parameters): %lf\n", (double)measure_time() / LOOPS);

    //three parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f3(i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (three parameters): %lf\n", (double)measure_time() / LOOPS);

    //four parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f4(i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (four parameters): %lf\n", (double)measure_time() / LOOPS);

    //five parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f5(i, i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (five parameters): %lf\n", (double)measure_time() / LOOPS);

    //six parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f6(i, i, i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (six parameters): %lf\n", (double)measure_time() / LOOPS);

    //seven parameters test
    MEASURE_START();
    for (int i = 0; i < LOOPS; i++) {
        f7(i, i, i, i, i, i, i);
    }
    MEASURE_END();
    printf("average procedure call overhead (seven parameters): %lf\n", (double)measure_time() / LOOPS);

}

void syscall_overhead() {
    MEASURE_START();
    getpid();
    MEASURE_END();
    printf("system call (getpid) overhead: %lf\n", (double)measure_time());
}

int main() {

	procedure_overhead();
	syscall_overhead();

	return 0;
}