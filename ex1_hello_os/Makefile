CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: hello_os hello_ring

hello_os: hello_os.c
	$(CC) $(CFLAGS) -o hello_os hello_os.c $(LDFLAGS)

hello_ring: hello_ring.c
	$(CC) $(CFLAGS) -o hello_ring hello_ring.c $(LDFLAGS)

run: all
	mpiexec --oversubscribe -n 8 ./hello_os
	mpiexec --oversubscribe -n 8 ./hello_ring

clean:
	rm -f hello_os hello_ring