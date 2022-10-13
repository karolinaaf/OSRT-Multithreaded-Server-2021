# -*- MakeFile -*-

#target: dependencies
#	action

CC = musl-gcc

all: client server

test: prototyp.o
	$(CC) prototyp.o -o test

client: client.o
	$(CC) client.o -o client

server: server.o
	$(CC) server.o -o server

prototyp.o: prototyp.c otazky.h scoring.h
	$(CC) -c prototyp.c

client.o: client.c
	$(CC) -c client.c

server.o: server.c otazky.h scoring.h
	$(CC) -c server.c

clean:
	rm -rf *.o prototyp server client
