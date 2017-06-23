#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include "Maxfiles.h"

//#define SIZE 1073741824
#define SIZE 2048
#define DEBUG 0

void ParallelPrefixSumCPU(int32_t *dataIn, int32_t *dataOut, size_t size) {
	dataOut[0] = dataIn[0];
	for (size_t i = 1; i < size; i++) {
		dataOut[i] = dataIn[i] + dataOut[i - 1];
	}
}

int main() {
	size_t dataSizeInBytes = SIZE * sizeof(int32_t);
	int32_t *dataIn = malloc(dataSizeInBytes);
	int32_t *dataOut = malloc(dataSizeInBytes);
	int32_t *dataExpected =  malloc(dataSizeInBytes);


	for (int i = 0; i < SIZE; i++) {
		dataIn[i] = (i%5)+1;
		dataOut[i] = 0;
	}

	struct timeval start, end;
	printf("Running CPU...\n");
	gettimeofday(&start, NULL);
	ParallelPrefixSumCPU(dataIn, dataExpected, SIZE);
	gettimeofday(&end, NULL);
	printf("CPU: %ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
				  - (start.tv_sec * 1000000 + start.tv_usec)));
	printf("End of CPU...\n");


	printf("Running DFE...\n");
	gettimeofday(&start, NULL);
	max_run_t *execStatus = ParallelPrefixSum_nonblock(SIZE, dataIn, dataOut);
	//ParallelPrefixSum(SIZE, dataIn, dataOut);
	max_wait(execStatus);
	gettimeofday(&end, NULL);
	printf("DFE: %ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
			  - (start.tv_sec * 1000000 + start.tv_usec)));

	printf("End of DFE...\n");
	if (DEBUG == 1) {
		for (int i = 0; i < SIZE; i++) {
			if (dataOut[i] == dataExpected[i]) {
				printf("dataExpected[%d] = dataOut[%d] = %d\n", i, i,
						dataOut[i]);
			} else {
				printf("dataExpected[%d] = %d\t\tdataOut[%d] = %d\n", i,
						dataExpected[i], i, dataOut[i]);
			}
		}
	}

	free(dataIn);
	free(dataOut);
	free(dataExpected);


	return 0;
}
