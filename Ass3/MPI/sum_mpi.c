#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int unitsize = 5;
    int* send_buffer = NULL;
    int receive_buffer[5];
    int* new_receive_buffer = NULL;
    int root = 0;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root) {
        int total_elements = unitsize * size;
        send_buffer = (int*)malloc(sizeof(int) * total_elements);
        printf("Enter %d elements\n", total_elements);
        for (int i = 0; i < total_elements; i++) {
            send_buffer[i] = i;
            printf("Element %d = %d\n", i, send_buffer[i]);
        }
    }

    // Scatter data to all processes
    MPI_Scatter(send_buffer, unitsize, MPI_INT,
                receive_buffer, unitsize, MPI_INT,
                root, MPI_COMM_WORLD);

    // Calculate partial sum
    int partial_sum = 0;
    for (int i = 0; i < unitsize; i++) {
        partial_sum += receive_buffer[i];
    }
    printf("Intermediate sum at process %d is %d\n", rank, partial_sum);

    // Gather partial sums at root
    if (rank == root) {
        new_receive_buffer = (int*)malloc(sizeof(int) * size);
    }

    MPI_Gather(&partial_sum, 1, MPI_INT,
               new_receive_buffer, 1, MPI_INT,
               root, MPI_COMM_WORLD);

    // Compute final sum at root
    if (rank == root) {
        int total_sum = 0;
        for (int i = 0; i < size; i++) {
            total_sum += new_receive_buffer[i];
        }
        printf("Final sum: %d\n", total_sum);
        free(send_buffer);
        free(new_receive_buffer);
    }

    MPI_Finalize();
    return 0;
}

