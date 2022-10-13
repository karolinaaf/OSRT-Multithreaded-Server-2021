# -*- MakeFile -*-

#target: dependencies
#	action

CC = musl-gcc

all: client server

test: main.o
	$(CC) main.o -o test

client: client.o
	$(CC) client.o -o client

server: server.o
	$(CC) server.o -o server

main.o: main.c otazky.h scoring.h
	$(CC) -c main.c

client.o: client.c
	$(CC) -c client.c

server.o: server.c otazky.h scoring.h
	$(CC) -c server.c

clean:
	rm -rf *.o test server client
