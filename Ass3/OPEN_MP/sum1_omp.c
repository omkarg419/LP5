#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n;
    int total_sum = 0;

    // Step 1: Get the array size from the user
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Step 2: Allocate memory for the array
    int* array = (int*)malloc(n * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Step 3: Get elements from the user
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Element %d = ", i);
        scanf("%d", &array[i]);
    }

    // Step 4: Parallel region to compute partial sum
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int chunk = n / nthreads;
        int start = tid * chunk;
        int end = (tid == nthreads - 1) ? n : start + chunk;

        int partial_sum = 0;
        for (int i = start; i < end; i++) {
            partial_sum += array[i];
        }

        printf("Thread %d: partial sum = %d\n", tid, partial_sum);

        // Safely update the shared total_sum
        #pragma omp atomic
        total_sum += partial_sum;
    }

    // Step 5: Print the final total sum
    printf("Final total sum: %d\n", total_sum);

    // Step 6: Free allocated memory
    free(array);

    return 0;
}

