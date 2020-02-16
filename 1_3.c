#include "cpu.h"
#include "pthread.h"
#include "sys/wait.h"

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

int main() {
	process_overhead();
    thread_overhead();
	return 0;
}