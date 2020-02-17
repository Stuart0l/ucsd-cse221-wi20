#include "cpu.h"

#define LOAD_NUM 1000000
#define LOG_SIZE_START 9

double time[30];

void measure_load(unsigned log_size, unsigned stride) {
	unsigned long size = 1 << log_size;
	unsigned long idx_msk = (1 << log_size) - 1;
	char *array;
	volatile register char r;
	unsigned long i;

	if ((array = malloc(size)) == NULL) {
		perror("alloc failed");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < size; i++)
		*(array + i) = i;

	MEASURE_START();
	for (i = 0; i < LOAD_NUM * stride; i+=stride) {
		r = *(array + (i&idx_msk));
	}
	MEASURE_END();

	printf("access size %d array takes: %lf cycles\n", size, (double)measure_time() / LOAD_NUM);

	time[log_size - LOG_SIZE_START] = (double)measure_time() / LOAD_NUM;

	free(array);
}

int main() {

	// warm up
	for (int i = 9; i < 30; i++)
			measure_load(i, 1);

	FILE *fd;
	fd = fopen("lat_mem.csv", "w");

	for (int s = 0; s < 10; s++) {
		int stride = 1 << s;
		printf("---stride: %d---\n", stride);
		for (int i = LOG_SIZE_START; i < 30; i++)
			measure_load(i, stride);
		fprintf(fd, "%d,", stride);
		for (int k = LOG_SIZE_START; k < 30; k++)
			fprintf(fd, "%f,", time[k-LOG_SIZE_START]);
		fprintf(fd, "\n");		
	}
	return 0;
}
