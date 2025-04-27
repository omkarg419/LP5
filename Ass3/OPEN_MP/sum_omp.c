#include <stdio.h>
#include <omp.h>

#define TOTAL_ELEMENTS 20
#define UNITSIZE 5

int main() {
    int array[TOTAL_ELEMENTS];
    int num_threads, total_sum = 0;

    // Initialize the array
    printf("Array elements:\n");
    for (int i = 0; i < TOTAL_ELEMENTS; i++) {
        array[i] = i;
        printf("Element %d = %d\n", i, array[i]);
    }

    // Parallel region to compute partial sums
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int chunk = TOTAL_ELEMENTS / nthreads;
        int start = tid * chunk;
        int end = (tid == nthreads - 1) ? TOTAL_ELEMENTS : start + chunk;

        int partial_sum = 0;
        for (int i = start; i < end; i++) {
            partial_sum += array[i];
        }

        printf("Thread %d: partial sum = %d\n", tid, partial_sum);

        // Add to total_sum with synchronization
        #pragma omp atomic
        total_sum += partial_sum;
    }

    printf("Final total sum: %d\n", total_sum);
    return 0;
}

