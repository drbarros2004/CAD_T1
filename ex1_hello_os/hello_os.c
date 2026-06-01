/*
 * Compile: mpicc -O2 -Wall -o hello_os hello_os.c
 * Run:     mpiexec -n 4 ./hello_os
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>   // getpid()
#include <sched.h>    // sched_getcpu()

#define MAX_STRING 100

int main(void) {
    char         greeting[MAX_STRING];
    int          comm_sz;   /* number of processes */
    int          my_rank;   /* this process's rank */
    pid_t        my_pid;    /* this process's id */
    int          my_cpu_core;   /* the core in which this process is running */

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    my_pid = getpid();
    my_cpu_core = sched_getcpu();

    if (my_rank != 0) {
        snprintf(greeting, MAX_STRING,
                 "Hello from rank %d/%d -- PID = %d, CPU = %d", my_rank, comm_sz, my_pid, my_cpu_core);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR,
                 0, 0, MPI_COMM_WORLD);
    } 
    else {
        printf("Hello from rank %d/%d -- PID = %d, CPU = %d\n", my_rank, comm_sz, my_pid, my_cpu_core);
        for (int q = 1; q < comm_sz; q++) {
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR,
                     q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    }

    MPI_Finalize();
    return 0;
}