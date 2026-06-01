/*
 * Compile: mpicc -O2 -Wall -o hello_gather hello_gather.c
 * Run:     mpiexec -n 4 ./hello_gather
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>   // getpid()
#include <sched.h>    // sched_getcpu()

#define MAX_STRING 100

int main(void) {
    char greeting[MAX_STRING];
    char *all_greetings = NULL;
    int comm_sz;
    int my_rank;
    pid_t my_pid;
    int my_cpu_core;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    my_pid = getpid();
    my_cpu_core = sched_getcpu();

    snprintf(greeting, MAX_STRING,
             "Hello from rank %d/%d -- PID = %d, CPU = %d", my_rank, comm_sz, my_pid, my_cpu_core);

    if (my_rank == 0) {
        all_greetings = (char*) malloc(comm_sz * MAX_STRING * sizeof(char));
    }

    MPI_Gather(greeting, MAX_STRING, MPI_CHAR,
               all_greetings, MAX_STRING, MPI_CHAR,
               0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        for (int i = 0; i < comm_sz; i++) {
            printf("%s\n", &all_greetings[i * MAX_STRING]);
        }
        free(all_greetings);
    }

    MPI_Finalize();
    return 0;
}