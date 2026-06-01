CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: mpi_vecadd_gather mpi_vecadd_allgather

mpi_vecadd_gather: mpi_vecadd_gather.c
	$(CC) $(CFLAGS) -o mpi_vecadd_gather mpi_vecadd_gather.c $(LDFLAGS)

mpi_vecadd_allgather: mpi_vecadd_allgather.c
	$(CC) $(CFLAGS) -o mpi_vecadd_allgather mpi_vecadd_allgather.c $(LDFLAGS)

run: all
	mpiexec --oversubscribe -n 8 ./mpi_vecadd_gather
	mpiexec --oversubscribe -n 8 ./mpi_vecadd_allgather

clean:
	rm -f mpi_vecadd_gather mpi_vecadd_allgather