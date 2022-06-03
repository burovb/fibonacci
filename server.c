#include "fib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#define PORT 8080
#define SA struct sockaddr
#define LEN 1024
#define THREADS 2
#define KEY 112837

struct msg_buffer {
    long msg_type;
    int msg;
} message;



int main() {
	int sockfd, newfd;
	socklen_t len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	bzero(&servaddr, sizeof servaddr);

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof servaddr)) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}

	// Now server is ready to listen and verification
	if ((listen(sockfd, 4)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}

	int msgid = msgget(KEY, 0666 | IPC_CREAT);

	while (1) {
		len = sizeof cli;
		newfd = accept(sockfd, (SA*)&cli, &len);
		if (newfd < 0) {
			printf("server accept failed...\n");
			exit(0);
		}

		if (!fork()) {
			close(sockfd);

			char buffer[LEN] = {0};
			read(newfd, buffer, LEN);

			size_t i;
    		pthread_t *id = malloc(sizeof *id * THREADS);
			time_t t;
			srand((unsigned) time(&t));
    		key_t key = rand();
    		int shmid = shmget(key, sizeof(int *), 0666|IPC_CREAT);
    		int *shm = shmat(shmid, (void*)0, 0); *shm = 0;
    		args_t *num = init(atoi(buffer), shm);

    		for (i = 0; i < THREADS; i++) {
				pthread_create(id + i, NULL, func, num + i);
			}	
    		for (i = 0; i < THREADS; i++) {
				pthread_join(*(id + i), NULL);
			}

			memset(buffer, 0, LEN);
			sprintf(buffer, "%d", *shm);
    		send(newfd, buffer, strlen(buffer), 0);

			message.msg_type = 1;
			message.msg = key;
			msgsnd(msgid, &message, sizeof message, 0);
    		shmdt(shm);
    		free(id);
    		free(num);

			close(newfd);
			exit(0);
		}
		close(newfd);
	}

	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}