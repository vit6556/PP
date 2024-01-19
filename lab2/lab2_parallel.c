#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char** argv) {
    const int count = 20000000;
    const int random_seed = 132957;
    const int max_threads = 24;
    const int iterations = 20;
    const int target = 16;
    double start_time, end_time, total;
    int* array;
    int index;

    srand(random_seed);
    printf("OpenMP version: %d\n", _OPENMP);

    for (int threads = 1; threads <= max_threads; threads++) {
        total = 0;

        for (int j = 0; j < iterations; ++j) {
            index = -1;

            array = (int*)malloc(count*sizeof(int));
            for (int i = 0; i < count; ++i) { 
                array[i] = rand();
            }

            start_time = omp_get_wtime();
            #pragma omp parallel num_threads(threads) reduction(min: index)
            {
                #pragma omp for
                for (int i = 0; i < count; ++i) {
                    if (array[i] == target) {
                        index = array[i];
                        i = count;
                    }
                }
            }
            end_time = omp_get_wtime();
            total += end_time - start_time;

            free(array);
        }
        printf("Threads: %d\tAvg time: %f\n", threads, total / (double) iterations);
    }

    return 0;
}