#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4455

int main() {

	int clientSocket_fd;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, '\0', sizeof(serverAddr));
	printf("[+]Successfully created new client socket.\n");

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(clientSocket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	printf("[+]Successfully connected to server socket.\n\n");

	printf("Enter bit stream: ");
	fgets(buffer, 1024, stdin);

	printf("[+]Sending bit stream....\n");
	recv(clientSocket_fd, buffer, 1024, 0);
	printf("Data recieved: %s\n", buffer);

	strcpy(buffer, "Client here.\0");
	send(clientSocket_fd, buffer, strlen(buffer), 0);
	recv(clientSocket_fd, buffer, 1024, 0);
	printf("Echo: %s\n", buffer);

	printf("Unstuffed bit stream: %s\n", "11111011111011111");
	printf("Stuffed bit stream: %s\n", stuffBitStream("11111011111011111"));
	return 0;
}