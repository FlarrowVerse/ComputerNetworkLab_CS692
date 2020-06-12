#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4455

char* stuffBitStream(char* bitStream) {
	char *stuffedStream = (char *)calloc(strlen(bitStream) + 2, sizeof(char));
	int i = 0, j = 0, count = 0;
	while (i < strlen(bitStream)) {
		stuffedStream[j] = bitStream[i];
		if (bitStream[i] == '0') {
			count = 0;
		} else {
			count++;
		}
		if (count == 5) {
			stuffedStream[++j] = '0';
		}
		i++;
		j++;
	}
	return stuffedStream;
}

int main() {

	int serverSocket_fd;
	struct sockaddr_in serverAddr;

	int newSocket_fd;
	struct sockaddr_in newAddr;

	socklen_t addr_size;
	char buffer[1024];

	serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("[+]Successfully created new server socket.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(serverSocket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	printf("[+]Successfully bound to port.\n");
	listen(serverSocket_fd, 5);
	printf("[+]Listening to port %d.....\n", PORT);

	addr_size = sizeof(newAddr);

	// for each new connection
	newSocket_fd = accept(serverSocket_fd, (struct sockaddr*)&newAddr, &addr_size);
	printf("[+]Accepted new client connection....\n");

	recv(newSocket_fd, buffer, 1024, 0);
	printf("[+]Recieved bit stream: %s\n", buffer);

	printf("\tStuffing bit stream.....");
	strcpy(buffer, stuffBitStream(buffer));

	// sending recieved data back
	printf("[+]Sending back stuffed bit stream....\n");
	send(newSocket_fd, buffer, strlen(buffer), 0);
	return 0;
}