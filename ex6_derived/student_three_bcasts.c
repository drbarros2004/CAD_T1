/*
 * Compile: mpicc -O2 -Wall -o student_three_bcasts student_three_bcasts.c
 * Run:     mpiexec -n 4 ./student_three_bcasts
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

struct Student {
    char name[50];
    double grade;
    int id;
};

int main(void) {
    int my_rank, comm_sz;
    struct Student student;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (my_rank == 0) {
        strcpy(student.name, "Daniel Barros");
        student.grade = 9.8;
        student.id = 10101;
    }

    MPI_Bcast(student.name, 50, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&student.grade, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&student.id, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank != 0) {
        printf("Process %d received: ID=%d, Name='%s', Grade=%.2f\n",
               my_rank, student.id, student.name, student.grade);
    }

    MPI_Finalize();
    return 0;
}