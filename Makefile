CFLAGS=-Wall -O3
CC=gcc

all: swparalelo swsequencial

clean:
	rm -f *.o swparalelo swsequencial

swparalelo: swParalelo.c
	$(CC) $< $(CFLAGS) -fopenmp -lm -o $@

swsequencial: swSequencial.c
	$(CC) $< $(CFLAGS) -o $@

