#include "../include/measure.h"

#define LOAD_NUM 100000
#define ARRAY_SIZE (1 << 29)
#define IDX_MSK (ARRAY_SIZE - 1)
#define LOG_MAX_STRIDE 12

double time[30];

void measure_load(unsigned stride, unsigned access_time[LOAD_NUM]) {

	char *array;
	volatile char r;
	unsigned int i;
	unsigned int idx = 0;

	if ((array = malloc(ARRAY_SIZE)) == NULL) {
		perror("alloc failed");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < ARRAY_SIZE; i++)
		*(array + i) = i;

	for (i = 0; i < LOAD_NUM; i++) {
		MEASURE_START();
		r = *(array + idx);
		MEASURE_END();
		access_time[i] = measure_time();
		idx = (idx + stride) & IDX_MSK;
	}

	free(array);
}

int main() {
	set_affinity();

	unsigned result[LOG_MAX_STRIDE][LOAD_NUM];

	FILE *fd;
	fd = fopen("lat_mem.csv", "w");

	for (int s = 0; s < LOG_MAX_STRIDE; s++) {
		int stride = 1 << s;
		printf("---stride: %d---\n", stride);
		measure_load(stride, result[s]);
	}

	for (int i = 0; i < LOG_MAX_STRIDE; i++) {
		fprintf(fd, "%d,", 1 << i);
		for (int j = 0; j < LOAD_NUM; j++)
			fprintf(fd, "%d,", result[i][j]);
		fprintf(fd, "\n");
	}

	fclose(fd);
	return 0;
}
