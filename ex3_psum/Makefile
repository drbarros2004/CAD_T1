CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: mpi_psum 

mpi_trap_generalized: mpi_psum.c
	$(CC) $(CFLAGS) -o mpi_psum mpi_psum.c $(LDFLAGS)

run: all
	mpiexec --oversubscribe -n 8 ./mpi_psum

clean:
	rm -f mpi_psum