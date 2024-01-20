#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>


void shellSort(int* arr, int n) {
    int i, gap;
    for (gap = n / 2; gap > 0; gap /= 2) {
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
    int rank, size;
    int* array;

    srand(random_seed);

    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int chunk = count / size;

    for (int j = 0; j < iterations; ++j) {
        int *cur_array, cur_size = count - chunk * (size - 1);
        MPI_Status status;

        if (rank == 0) { // если основной поток, то нужно разослать всем части массива
            array = (int*)malloc(count*sizeof(int));
            for (int i = 0; i < count; ++i) { 
                array[i] = rand();
            }

            // рассылаем части массива всем потокам
            for (int dest = 1; dest < size; dest++) {
                MPI_Send(array + chunk * (dest - 1), chunk, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }

            cur_array = malloc(cur_size * sizeof(int));
            int *sorted = malloc(count * sizeof(int));

            // сортируем свою часть массива
            start_time = MPI_Wtime();
            for (int i = chunk * (size - 1), j = 0; i < count; i++, j++) {
                cur_array[j] = array[i];
            }
            shellSort(cur_array, cur_size);
            memcpy(sorted, cur_array, cur_size * sizeof(int));

            // получаем отсортированные части от других потоков
            for (int src = 1; src < size; src++) {
                MPI_Recv(sorted + chunk * src, cur_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            }

            // сортируем итоговый массив
            shellSort(sorted, count);
            end_time = MPI_Wtime();

            free(sorted);
            free(cur_array);
        } else { // если поток не основное, то получаем массив, сортируем и отправляем обратно
            cur_array = malloc(chunk * sizeof(int));

            MPI_Recv(cur_array, chunk, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            shellSort(cur_array, chunk);
            MPI_Send(cur_array, chunk, MPI_INT, 0, 1, MPI_COMM_WORLD);

            free(cur_array);
        }

        total += end_time - start_time;
    }

    if (rank == 0) {
        printf("Threads: %d\tAvg time: %f\n", size, total / (double) iterations);
    }
    
    MPI_Finalize();

    return 0;
}