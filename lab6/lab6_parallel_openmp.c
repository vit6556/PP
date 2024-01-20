#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void shellSort(int* arr, int n, int threads) {
    int i, gap;
    for (gap = n / 2; gap > 0; gap /= 2) {
        #pragma omp parallel for shared(arr, gap, n) private(i) num_threads(threads)
        for (i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

int main(int argc, char** argv) {
    const int count = 1000000;
    const int random_seed = 132957;
    const int max_threads = 24;
    const int iterations = 20;
    double start_time, end_time, total;
    int* array;

    srand(random_seed);
    printf("OpenMP version: %d\n", _OPENMP);

    for (int threads = 1; threads <= max_threads; threads++) {
        total = 0;

        for (int j = 0; j < iterations; ++j) {
            array = (int*)malloc(count*sizeof(int));
            for (int i = 0; i < count; ++i) { 
                array[i] = rand();
            }

            start_time = omp_get_wtime();
            shellSort(array, count, threads);
            end_time = omp_get_wtime();
            total += end_time - start_time;

            free(array);
        }
        printf("Threads: %d\tAvg time: %f\n", threads, total / (double) iterations);
    }

    return 0;
}