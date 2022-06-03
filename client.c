#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define SA struct sockaddr
#define LEN 1024



int main() {
	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	bzero(&servaddr, sizeof servaddr);

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof servaddr) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}

	char buffer[LEN] = {0};
	fgets(buffer, LEN, stdin);
	send(sockfd, buffer, strlen(buffer), 0);
	memset(buffer, 0, LEN);
	read(sockfd, buffer, LEN);
	printf("%s\n", buffer);

	// close the socket
	close(sockfd);
	return 0;
}
