#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LOOPS 1000

// Benchmark functions
unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

#define MEASURE_START() \
    asm volatile ("CPUID\n\t" \
                  "RDTSC\n\t" \
                  "mov %%edx, %0\n\t" \
                  "mov %%eax, %1\n\t" \
                  : "=r" (cycles_high), "=r" (cycles_low) \
                  :: "%rax", "%rbx", "%rcx", "%rdx");

#define MEASURE_END() \
    asm volatile ("RDTSCP\n\t" \
                  "mov %%edx, %0\n\t" \
                  "mov %%eax, %1\n\t" \
                  "CPUID\n\t" \
                  : "=r" (cycles_high1), "=r" (cycles_low1) \
                  :: "%rax", "%rbx", "%rcx", "%rdx");

uint64_t measure_time() {
    uint64_t start, end;
    start = ((uint64_t)cycles_high << 32) | cycles_low;
    end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
    if (end < start) printf("ERROR: start-%lu, end-%lu\n", start, end);
    return end - start;
}

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

void process_overhead() {
    uint64_t sum = 0;
    for (int i = 0; i < LOOPS; i++) {
        MEASURE_START();
        pid_t pid = fork();
        if (pid > 0) { MEASURE_END(); }
        else if (pid == -1) {
            printf("fork failure\n");
            exit(1);
        }
        if (pid == 0) exit(0);
        sum += measure_time();
        wait(NULL);
    }
    printf("average process creation overhead: %lf\n", (double)sum / LOOPS);
}

void thread_func() {}

void thread_overhead() {
    pthread_t thread;
    uint64_t sum = 0;

    for (int i = 0; i < LOOPS; i++) {
        MEASURE_START();
        if (pthread_create(&thread, NULL, (void * (*)(void *))thread_func, NULL) != 0) {
            printf("pthread_create failure\n");
            exit(1);
        }
        MEASURE_END();
        if (pthread_join(thread, NULL) != 0) {
            printf("pthread_join failure\n");
            exit(1);
        }
        sum +=measure_time();
    }
    
    printf("average thread creation overhead: %lf\n", (double)sum / LOOPS);
}

// pipe for context switch
static int fd[2];

void process_switch_overhead() {
    double sum = 0;
    uint64_t end_time;

    for (int i = 0; i < LOOPS; ) {
        pid_t pid = fork();
        if (pid == -1) {
            printf("fork failure\n");
            exit(1);
        }
        else if (pid == 0) {
            MEASURE_END();
            end_time = ((uint64_t)cycles_high1 << 32) | cycles_low1;
            write(fd[1], &end_time, sizeof(uint64_t));
            exit(0);
        }
        else {
            MEASURE_START();
            read(fd[0], &end_time, sizeof(uint64_t));
            uint64_t start_time = ((uint64_t)cycles_high << 32) | cycles_low;
            if (end_time > start_time) {
                sum += end_time - start_time;
                i++;
            }
            wait(NULL);     // wait the child to exit
        }
    }
    printf("average process switch overhead: %lf\n", sum / LOOPS);
}

void thread_switch() {
    MEASURE_END();
    int buf;
    write(fd[1], &buf, sizeof(int));
}

void thread_switch_overhead() {
    double sum = 0;
    pthread_t thread;
    int buf;
    for (int i = 0; i < LOOPS; ) {
        if (pthread_create(&thread, NULL, (void * (*)(void *))thread_switch, NULL) != 0) {
            printf("pthread_create failure\n");
            exit(0);
        }
        MEASURE_START();
        read(fd[0], &buf, sizeof(int));
        if (pthread_join(thread, NULL) != 0) {
            printf("pthread_join failure\n");
            exit(0);
        }
        uint64_t res = measure_time();
        if (res > 0) {
            sum += res;
            i++;
        }
    }
    printf("average thread switch overhead: %lf\n", sum / LOOPS);
}

int main(int argc, char const *argv[]) {
    if (pipe(fd) != 0) {
        printf("failed to open the pipe\n");
        exit(0);
    }
    // cycle_time();
    reading_overhead();
    loop_overhead();
    procedure_overhead();
    syscall_overhead();
    process_overhead();
    thread_overhead();
    process_switch_overhead();
    thread_switch_overhead();
    return 0;
}