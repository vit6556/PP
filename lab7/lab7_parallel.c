#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>


bool isPrime(int number, int *primes) {
    bool prime = true;
    int check_amount = (int) sqrt(number);
    if (number <= 1) return false;

    #pragma omp parallel shared(prime, check_amount)
    {
        #pragma omp for
        for (int i = 2; i <= check_amount; i++) {
            if (number % i == 0) {
                prime = false;
                i = check_amount + 1;
            }
        }
    }

    if (prime) {
        #pragma omp critical
        {
            *primes += 1;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int start = 1;
    int end = 100000000;

    int primes = 0, result = 0;
    double start_time, end_time;

    int chunk = (end - start + 1) / size;
    int rem = (end - start + 1) % size;

    int local_start = start + rank * chunk;
    int local_end = local_start + chunk - 1;

    if (rank < rem) {
        local_start += rank;
        local_end += 1;
    } else {
        local_start += rem;
    }

    int max_threads = omp_get_max_threads();
    int threads = (size > max_threads) ? max_threads / size : 2;
    omp_set_num_threads(threads);
    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();
    #pragma omp parallel for
    for (int n = local_start; n <= local_end; n++) {
        isPrime(n, &result);
    }
    end_time = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    double work_time = end_time - start_time;
    double *work_times;
    if (rank == 0) {
        work_times = (double *) malloc(size * sizeof(double));
    }

    MPI_Gather(&work_time, 1, MPI_DOUBLE, work_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Process\tTime\n");
        for (int i = 0; i < size; i++) {
            printf("%d\t%f\n", i, work_times[i]);
        }
    }

    MPI_Finalize();

    return 0;
}