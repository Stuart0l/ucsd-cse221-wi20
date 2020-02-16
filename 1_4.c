#include "cpu.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

// pipe for context switch
static int fd[2];

void process_switch_overhead() {
    double sum = 0;
    uint64_t end_time;

    for (int i = 0; i < LOOPS; ) {
        pid_t pid = fork();
        
        if (pid == 0) {
            MEASURE_END();
            end_time = ((uint64_t)cycles_high1 << 32) | cycles_low1;
            write(fd[1], &end_time, sizeof(uint64_t));
            exit(0);
        } else if (pid > 0) {
            MEASURE_START();
            read(fd[0], &end_time, sizeof(uint64_t));
            uint64_t start_time = ((uint64_t)cycles_high << 32) | cycles_low;
            if (end_time > start_time) {
                sum += end_time - start_time;
                i++;
            }
            wait(NULL);     // wait the child to exit
        } else {
            printf("fork failure\n");
            exit(1);
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

    for (int i = 0; i < 20; ++i)
    {
        thread_switch_overhead();
        printf("------------------------------\n");
    }

    process_switch_overhead();
    thread_switch_overhead();
    return 0;
}
