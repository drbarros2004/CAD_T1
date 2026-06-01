/*
 * Compile: mpicc -O2 -Wall -o student_struct student_struct.c
 * Run:     mpiexec -n 4 ./student_struct
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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

    int blocklengths[3] = {50, 1, 1};
    MPI_Aint displacements[3];
    MPI_Datatype types[3] = {MPI_CHAR, MPI_DOUBLE, MPI_INT};
    MPI_Datatype mpi_student_type;

    displacements[0] = offsetof(struct Student, name);
    displacements[1] = offsetof(struct Student, grade);
    displacements[2] = offsetof(struct Student, id);

    MPI_Type_create_struct(3, blocklengths, displacements, types, &mpi_student_type);
    MPI_Type_commit(&mpi_student_type);

    if (my_rank == 0) {
        strcpy(student.name, "Daniel Barros");
        student.grade = 9.8;
        student.id = 10101;
    }

    MPI_Bcast(&student, 1, mpi_student_type, 0, MPI_COMM_WORLD);

    if (my_rank != 0) {
        printf("Process %d received: ID=%d, Name='%s', Grade=%.2f\n",
               my_rank, student.id, student.name, student.grade);
    }

    MPI_Type_free(&mpi_student_type);
    MPI_Finalize();
    return 0;
}