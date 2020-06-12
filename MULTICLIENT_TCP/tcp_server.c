#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define MAX_CONN 50

char* addParityBit(char *bitStream) {
	char *modBitStream = (char*)calloc(strlen(bitStream) + 1, sizeof(char));
	int i = 0, j = 1;
	char flag = '0';
	while(i < strlen(bitStream)) {
		if (bitStream[i] == '1') {
			if (flag == '1') {
				flag = '0';
			} else {
				flag = '1';
			}
		}
		modBitStream[j] = bitStream[i];
		i++; j++;
	}
	modBitStream[0] = flag;

	return modBitStream;
}

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, MAX_CONN) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strncmp(buffer, ":exit", 5) == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client Request: %s\n", buffer);
					strcpy(buffer, addParityBit(buffer));
					printf("Sending response to %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, 1024);
				}
			}
		}

	}

	close(newSocket);


	return 0;
}