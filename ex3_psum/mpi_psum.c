#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define N 10000000 

int main(void) {
    int my_rank, comm_sz;
    long local_N;
    double *global_vec = NULL;
    double *local_vec = NULL;
    double local_sum = 0.0, total_sum = 0.0, serial_sum = 0.0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    local_N = N / comm_sz;

    local_vec = (double*) malloc(local_N * sizeof(double));

    // Process with rank = 0 generates data and distributes it 
    if (my_rank == 0) {
        global_vec = (double*) malloc(N * sizeof(double));
        srand(time(NULL));

        for (long i = 0; i < N; i++) {
            global_vec[i] = (double)rand() / RAND_MAX;
            serial_sum += global_vec[i];
        }

        for (int dest = 1; dest < comm_sz; dest++) {
            MPI_Send(&global_vec[dest * local_N], local_N, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        }

        for (long i = 0; i < local_N; i++) {
            local_vec[i] = global_vec[i];
        }
    } else {
        // Other processess receive their N/p elements blocks
        MPI_Recv(local_vec, local_N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // All processess compute local sum
    for (long i = 0; i < local_N; i++) {
        local_sum += local_vec[i];
    }

    // Process 0 gathers all partial sums
    if (my_rank != 0) {
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        total_sum = local_sum; 
        
        for (int src = 1; src < comm_sz; src++) {
            double partial_sum;
            MPI_Recv(&partial_sum, 1, MPI_DOUBLE, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_sum += partial_sum;
        }

        // Precision and printing analysis
        double rel_error = fabs(total_sum - serial_sum) / serial_sum;
        printf("Serial sum     = %.15e\n", serial_sum);
        printf("Parallel sum   = %.15e\n", total_sum);
        printf("Relative error = %.15e\n", rel_error);

        free(global_vec); 
    }

    free(local_vec); 
    MPI_Finalize();
    return 0;
}