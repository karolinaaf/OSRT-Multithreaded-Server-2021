// Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Sockety
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Vlákna
#include <pthread.h>

// Časovače
#include <time.h>

// Signály
#include <signal.h>

// Moje veci
#include "otazky.h"
#include "scoring.h"

#define BACKLOG 5 // Na koľko pending spojení je server ochotný čakať
#define BUFFSIZE 128
#define TIMEOUT 300 // Čas v sekundách dokým server nespácha samovraždu ak sa správne neukončí

int qnum;

void* handle_client(void*);
void set_timer(timer_t);
timer_t create_timer(int);

int main(int argc, char *argv[]) {
	if (argc == 3) {
		qnum = atoi(argv[2]); // Number of questions

		if (qnum > 50 || qnum < 1) {
			printf("Invalid number of questions entered. Did you read  the documentation?\n");
			return 0;
		}

		printf("%s running on port %s using %d questions.\n", argv[0], argv[1], qnum);
	}
	else if (argc > 3) {
		printf("Too many arguments supplied. Did you read the documentation?\n");
		return 0;
	}
	else {
		printf("Two arguments expected. Did you read the documentation?\n");
		return 0;
	}

	/* Vytvorenie socketu */
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		printf("Failed to create socket\n");
		return 0;
	}

	/* Adresy a bindovanie */
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
		printf("Failed to bind socket\n");
		close(server_socket);
		return 0;
	}

	/* Listening */
	if (listen(server_socket, BACKLOG) != 0) {
		printf("Failed to listen to socket\n");
		close(server_socket);
		return 0;
	}

	/* Staranie sa o spojenia */
	pthread_t thread[4];

	for (int i = 0; i < 4; i++) { // Server prijme štyri spojenia
		printf("Waiting for client %d...\n", i);
		int client_socket = accept(server_socket, NULL, NULL);

		if (client_socket == -1) {
			printf("Failed to accept client %d\n", i);
			close(client_socket);
			continue;
		}
		else
			printf("Handling client %d...\n", i);
		
		int* pclient = malloc(sizeof(int));
		*pclient = client_socket;

		if (pthread_create(&thread[i], NULL, handle_client, pclient) != 0)
			printf("Failed to create thread %d\n", i);
	}

	/* Zabitie programu v prípade, ak sa správne neukončí */
	timer_t my_timer = create_timer(SIGKILL);
	set_timer(my_timer);

	/* Ukončenie programu */
	for (int i = 0; i < 4; i++)
		pthread_join(thread[i], NULL); // Počká na všetky vlákna a potom po nich uprace
	
	close(server_socket);

	printf("%s halted.\n", argv[0]);
	return 0;
}

void* handle_client(void* pclient) {
	int client_socket = *((int*)pclient);
	free(pclient);

	char buff[BUFFSIZE];

	for (int i = 0; i < qnum; i++) {
		// Odosielanie otázok
		send(client_socket, &quest[i], sizeof(buff), 0);
		recv(client_socket, &buff, sizeof(buff), 0);
		
		// Odosielanie scoringu
		for (int j = 0; j < 2; j++) {
			int temp = htonl(scl_dir[i][j]);
			send(client_socket, &temp, sizeof(temp), 0);
			recv(client_socket, &buff, sizeof(buff), 0);
		}
	}

	// Server oznámi klientovi, že ukončuje komunikáciu
	send(client_socket, "end", sizeof("end"), 0);

	printf("Closing connection...\n");
	close(client_socket);

	return NULL;
}

void set_timer(timer_t my_timer) {
	struct itimerspec my_time;

	my_time.it_value.tv_sec = TIMEOUT;
	my_time.it_value.tv_nsec = 0;
	my_time.it_interval.tv_sec = 0;
	my_time.it_interval.tv_nsec = 0;

	timer_settime(my_timer, CLOCK_REALTIME, &my_time, NULL);
}

timer_t create_timer(int signal) {
	struct sigevent sig_event;

	sig_event.sigev_notify = SIGEV_SIGNAL;
	sig_event.sigev_signo = signal;

	timer_t my_timer;
	timer_create(CLOCK_REALTIME, &sig_event, &my_timer);

	return my_timer;
}