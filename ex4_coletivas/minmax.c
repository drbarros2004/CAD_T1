/*
 * Compile: mpicc -O2 -Wall -o minmax minmax.c
 * Run:     mpiexec -n 4 ./minmax
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 16 

int main(void) {
    int my_rank, comm_sz;
    double *x = NULL;
    double global_max, global_min, serial_max = 0.0, serial_min = 0.0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (N % comm_sz != 0) {
        if (my_rank == 0) fprintf(stderr, "N=%d should be divisible by p=%d\n", N, comm_sz);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int local_n = N / comm_sz;
    double local_x[local_n];

    if (my_rank == 0) {
        x = (double *) malloc(N * sizeof(double));
        srand(time(NULL));
        
        x[0] = ((double)rand() / RAND_MAX) * 100.0;
        serial_max = x[0];
        serial_min = x[0];
        
        for (int i = 1; i < N; i++) {
            x[i] = ((double)rand() / RAND_MAX) * 100.0;
            if (x[i] > serial_max) serial_max = x[i];
            if (x[i] < serial_min) serial_min = x[i];
        }
    }

    // Scatter the slices
    MPI_Scatter(x, local_n, MPI_DOUBLE, 
                local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Local search
    double local_max = local_x[0];
    double local_min = local_x[0];
    for (int i = 1; i < local_n; i++) {
        if (local_x[i] > local_max) local_max = local_x[i];
        if (local_x[i] < local_min) local_min = local_x[i];
    }

    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("Max: Serial = %.6f | Parallel = %.6f\n", serial_max, global_max);
        printf("Min: Serial = %.6f | Parallel = %.6f\n", serial_min, global_min);
        free(x);
    }

    MPI_Finalize();
    return 0;
}