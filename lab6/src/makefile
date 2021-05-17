CC=gcc
CFLAGS=-I.

all:client server

client: client.c 
	$(CC) -o client -lpthread client.c $(CFLAGS)

server: server.c 
	$(CC) -o server -lpthread server.c $(CFLAGS)

clean:
	rm client server