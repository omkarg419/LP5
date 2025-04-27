#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int *full_array = NULL;
    int *sub_array;
    int *partial_sums = NULL;
    int unitsize = 2;
    int total_elements;

    MPI_Init(&argc, &argv);                 // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get current process id
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get number of processes

    total_elements = unitsize * size;
    sub_array = (int*) malloc(unitsize * sizeof(int));

    if (rank == 0) {
        full_array = (int*) malloc(total_elements * sizeof(int));
        printf("Enter %d elements:\n", total_elements);
        for (int i = 0; i < total_elements; i++) {
            printf("Element [%d]: ", i);
            scanf("%d", &full_array[i]);
        }
    }

    // Scatter the array to all processes
    MPI_Scatter(full_array, unitsize, MPI_INT,
                sub_array, unitsize, MPI_INT,
                0, MPI_COMM_WORLD);

    // Each process computes partial sum
    int local_sum = 0;
    for (int i = 0; i < unitsize; i++) {
        local_sum += sub_array[i];
    }

    printf("Process %d: Local sum = %d\n", rank, local_sum);

    // Root process gathers partial sums
    if (rank == 0) {
        partial_sums = (int*) malloc(size * sizeof(int));
    }

    MPI_Gather(&local_sum, 1, MPI_INT,
               partial_sums, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    // Root process computes final sum
    if (rank == 0) {
        int total_sum = 0;
        for (int i = 0; i < size; i++) {
            total_sum += partial_sums[i];
        }
        printf("Final total sum = %d\n", total_sum);
        free(full_array);
        free(partial_sums);
    }

    free(sub_array);
    MPI_Finalize();
    return 0;
}

