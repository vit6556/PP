#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    const int count = 100;
    const int random_seed = 132957;

    srand(random_seed);

    int sum = 0;
    int *array = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) { 
        array[i] = rand();
    }

    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        omp_set_lock(&lock);
        sum += array[i];
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    printf("Sum: %d\n", sum);

    free(array);
    return 0;
}