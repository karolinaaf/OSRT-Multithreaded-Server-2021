// Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Sockety
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// Signály
#include <signal.h>

// Procesy
#include <sys/wait.h>

#define BUFFSIZE 128

int main(int argc, char *argv[]) {
  if (argc == 2) {
		printf("%s running on port %s\n", argv[0], argv[1]);
	}
	else if (argc > 2) {
		printf("Too many arguments supplied. Did you read the documentation?\n");
		return 0;
	}
	else {
		printf("One arguments expected. Did you read the documentation?\n");
		return 0;
	}

  int pipe_fd[2];
  if (pipe(pipe_fd) == -1) {
    printf("Failed to open pipe.\n");
    return 0;
  }

  pid_t pid = fork();

  /* Parent process, komunikácia so serverom */
  if (pid > 0) {
    close(pipe_fd[0]);

    // Vytvorenie socketu
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket == -1) {
      printf("Failed to create socket\n");
      return 0;
    }

    // Nastavenie adresy socketu
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Pripojenie na serverový socket
    if (connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
      printf("Failed to connect to the remote socket\n");
      close(network_socket);
      return 0;
    }

    // Prijímanie správ
    char buff[BUFFSIZE];
    int temp_answer, temp_scl_dir[2];

    while(1) {
      // Prijímanie otázky / end signálu
      recv(network_socket, &buff, sizeof(buff), 0);

      if (!strcmp(buff, "end")) {
        printf("All questions have been served, server disconected.\n");

        temp_answer = 20;
        write(pipe_fd[1], &temp_answer, sizeof(temp_answer));
        kill(pid, SIGCONT);

        break;
      }
      else {
        printf("%s\n", buff); // Vypísanie otázky

        // Program si bude žiadať novú odpoveď pokým užívateľ nezadá odpoveď v platnom rozsahu
        do {
          printf("Answer:\n");
          scanf("%d", &temp_answer);
        } while (temp_answer > 5 || temp_answer < 1);

        send(network_socket, &buff, sizeof(buff), 0);

        write(pipe_fd[1], &temp_answer, sizeof(temp_answer)); // Preposlanie užívateľskej odpovede child procesu
        
        // Prijímanie scoringu
        for (int i = 0; i < 2; i++) {
          recv(network_socket, &temp_scl_dir[i], sizeof(temp_scl_dir[i]), 0);
          send(network_socket, &buff, sizeof(buff), 0);

          int temp = ntohl(temp_scl_dir[i]);
          write(pipe_fd[1], &temp, sizeof(temp));
        }

        kill(pid, SIGCONT);
      }
    }

    wait(NULL);

    close(network_socket);
    close(pipe_fd[1]);
  }

  /* Child process, spracúvanie prijatých dát */
  else if (pid == 0) {
    close(pipe_fd[1]);

    /*
    (1) Extraversion
    (2) Agreeableness
    (3) Conscientiousness
    (4) Emotional Stability
    (5) Intellect/Imagination
    */
    
    int extra = 0, agree = 0, consc = 0, emot = 0, intel = 0;
    int answer, scl_dir[2];

    while(1) {
      raise(SIGSTOP); // ZA WARUDO!

      read(pipe_fd[0], &answer, sizeof(answer)); // Prečítanie odpovede

      if (answer > 5)
        break;

      for (int i = 0; i < 2; i++)
        read(pipe_fd[0], &scl_dir[i], sizeof(scl_dir[i])); // Prečítanie scoringu

      switch (scl_dir[0]) {
        case 1: // (1) Extraversion
          if (scl_dir[1]) extra += answer;
          else extra += 6 - answer;
          break;
        
        case 2: // (2) Agreeableness
          if (scl_dir[1]) agree += answer;
          else agree += 6 - answer;
          break;
        
        case 3: // (3) Conscientiousness
          if (scl_dir[1]) consc += answer;
          else consc += 6 - answer;
          break;

        case 4: // (4) Emotional Stability
          if (scl_dir[1]) emot += answer;
          else emot += 6 - answer;
          break;

        case 5: // (5) Intellect/Imagination
          if (scl_dir[1]) intel += answer;
          else intel += 6 - answer;
          break;

        default:
          printf("Scoring error.\n");
          break;
		  }
    }

    close(pipe_fd[0]);

    printf("\nExtraversion = %d\nAgreeableness = %d\nConscientiousness = %d\nEmotional Stability = %d\nIntellect/Imagination = %d\n", extra, agree, consc, emot, intel);
  }

  /* Správa v prípade zlyhania forku */
  else {
    printf("Fork failed. Returning 1...\n");
    return 1;
  }

  return 0;
}
