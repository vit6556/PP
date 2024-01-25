#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>


bool isPrime(int number, int *primes) {
    int check_amount = (int) sqrt(number);
    if (number <= 1) return false;

    for (int i = 2; i <= check_amount; i++) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    int start = 1;
    int end = 100000000;

    int primes = 0;
    double start_time, end_time;

    start_time = omp_get_wtime();

    for (int n = start; n <= end; n++) {
        primes += isPrime(n, &primes);
    }

    end_time = omp_get_wtime();

    printf("Prime numbers amount between %d and %d: %d\n", start, end, primes);
    printf("Execution time: %f\n", end_time - start_time);

    return 0;
}