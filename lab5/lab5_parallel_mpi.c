#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char** argv) {
    const int count = 20000000;
    const int random_seed = 132957;
    const int max_threads = 24;
    const int iterations = 10;
    double start_time, end_time, total;
    int max, rank, size, chunk;
    int* array = (int*)malloc(count*sizeof(int));

    srand(random_seed);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    chunk = count / size;

    for (int j = 0; j < iterations; ++j) {
        if (rank == 0) {
            for (int i = 0; i < count; ++i) { 
                array[i] = rand();
            }
        }

        MPI_Bcast(array, count, MPI_INT, 0, MPI_COMM_WORLD);

        start_time = MPI_Wtime();
        int cur_max = array[0];
        for (int i = chunk * rank; i < count && i < chunk * (rank + 1); i++) {
            if (array[i] > cur_max) {
                cur_max = array[i];
            }
        }

        MPI_Reduce(&cur_max, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        end_time = MPI_Wtime();
        total += end_time - start_time;
    }
    if (rank == 0) {
        printf("Threads: %d\tAvg time: %f\n", size, total / (double) iterations);
    }

    MPI_Finalize();
    free(array);
    return 0;
}