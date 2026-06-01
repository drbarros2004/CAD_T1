CC = mpicc
CFLAGS = -O2 -Wall
LDFLAGS = -lm

all: student_struct student_three_bcasts

student_struct: student_struct.c
	$(CC) $(CFLAGS) -o student_struct student_struct.c $(LDFLAGS)

student_three_bcasts: student_three_bcasts.c
	$(CC) $(CFLAGS) -o student_three_bcasts student_three_bcasts.c $(LDFLAGS)

run: all
	mpiexec --oversubscribe -n 8 ./student_struct
	mpiexec --oversubscribe -n 8 ./student_three_bcasts

clean:
	rm -f student_struct student_three_bcasts