#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(int argc, char** argv) {
    const int count = 20000000;
    const int random_seed = 132957;
    const int iterations = 20;
    double start_time, end_time, total = 0;
    int* array;
    int max;

    srand(random_seed);
    printf("OpenMP version: %d\n", _OPENMP);

    for (int j = 0; j < iterations; ++j) {
        max = -1;

        array = (int*)malloc(count*sizeof(int));
        for (int i = 0; i < count; ++i) { 
            array[i] = rand();
        }

        start_time = omp_get_wtime();
        for (int i = 0; i < count; ++i) {
            if (array[i] > max) {
                max = array[i];
            }
        }
        end_time = omp_get_wtime();
        total += end_time - start_time;

        free(array);
    }

    printf("Avg time: %f\n", total / (double) iterations);
    return 0;
}