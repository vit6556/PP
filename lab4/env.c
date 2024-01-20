#include <stdio.h>
#include <omp.h>

int main() {
    printf("OpenMP Version %d\n", _OPENMP);
    printf("Number of processors: %d\n", omp_get_num_procs());
    printf("Maximum number of threads: %d\n", omp_get_max_threads());
    printf("Dynamic threads are %s\n", omp_get_dynamic() ? "enabled" : "disabled");
    printf("Timer resolution: %g seconds\n", omp_get_wtick());
    printf("Nested parallelism is %s\n", omp_get_nested() ? "enabled" : "disabled");
    printf("Max active levels: %d\n", omp_get_max_active_levels());

    omp_sched_t kind;
    int chunk_size;
    omp_get_schedule(&kind, &chunk_size);
    printf("Schedule: %d, chunk size: %d\n", kind, chunk_size);
}