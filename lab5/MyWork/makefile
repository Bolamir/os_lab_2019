CC=gcc
CFLAGS=-I.

all:mutex factorial

mutex: mutex.c 
	$(CC) -o mutex -lpthread mutex.c $(CFLAGS)

factorial: factorial.c 
	$(CC) -o factorial -lpthread factorial.c $(CFLAGS)

clean:
	rm mutex factorial