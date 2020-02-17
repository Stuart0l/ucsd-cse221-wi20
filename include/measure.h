#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#define LOOPS 1000

unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

#define MEASURE_START() \
    asm volatile ("CPUID\n\t" \
                  "RDTSC\n\t" \
                  "mov %%edx, %0\n\t" \
                  "mov %%eax, %1\n\t" \
                  : "=r" (cycles_high), "=r" (cycles_low) \
                  :: "%rax", "%rbx", "%rcx", "%rdx")

#define MEASURE_END() \
    asm volatile ("RDTSCP\n\t" \
                  "mov %%edx, %0\n\t" \
                  "mov %%eax, %1\n\t" \
                  "CPUID\n\t" \
                  : "=r" (cycles_high1), "=r" (cycles_low1) \
                  :: "%rax", "%rbx", "%rcx", "%rdx")

uint64_t measure_time() {
    uint64_t start, end;
    start = ((uint64_t)cycles_high << 32) | cycles_low;
    end = ((uint64_t)cycles_high1 << 32) | cycles_low1;
    if (end < start) printf("ERROR: start-%lu, end-%lu\n", start, end);
    return end - start;
}
