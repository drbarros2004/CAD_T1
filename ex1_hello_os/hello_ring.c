/* 
 * Compile: mpicc -g -Wall -o hello_ring hello_ring.c
 * Run:     mpiexec -n 8 ./hello_ring
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>   // getpid()
#include <sched.h>    // sched_getcpu()

#define MAX_STRING 100

int main(void) {
    char send_greeting[MAX_STRING];
    char recv_greeting[MAX_STRING];
    int          comm_sz;       /* number of processes */
    int          my_rank;       /* this process's rank */
    pid_t        my_pid;        /* this process's id */
    int          my_cpu_core;   /* the core in which this process is running */
    int          neighbor_to_send_rank;
    int          neighbor_to_recv_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    my_pid = getpid();
    my_cpu_core = sched_getcpu();

    neighbor_to_send_rank = (my_rank + 1) % comm_sz;
    neighbor_to_recv_rank = (my_rank - 1 + comm_sz) % comm_sz;
    
    snprintf(send_greeting, MAX_STRING,
                "Hello from rank %d/%d -- PID = %d, CPU = %d", my_rank, comm_sz, my_pid, my_cpu_core);

    if (my_rank == 0) {
        printf("%s\n", send_greeting);
    }

    for (int i = 1; i < comm_sz; i++) {
        MPI_Sendrecv(
            send_greeting, strlen(send_greeting) + 1, MPI_CHAR, neighbor_to_send_rank, 0,  
            recv_greeting, MAX_STRING, MPI_CHAR, neighbor_to_recv_rank, 0,       
            MPI_COMM_WORLD, MPI_STATUS_IGNORE                   
        );
        if (my_rank == 0) {
            printf("%s\n", recv_greeting);
        }
        strncpy(send_greeting, recv_greeting, MAX_STRING);
    }


    MPI_Finalize();
    return 0;
}