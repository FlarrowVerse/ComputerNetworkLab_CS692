#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define MAXDATA 64

int main(){

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		printf("Enter dataword: ");
		fgets(buffer, MAXDATA, stdin);
		buffer[strlen(buffer)-1] = '\0';
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strncmp(buffer, "0", 1) == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		strcpy(buffer, "");
		printf("Enter divisor: ");
		fgets(buffer, MAXDATA, stdin);
		buffer[strlen(buffer)-1] = '\0';
		send(clientSocket, buffer, strlen(buffer), 0);
		

		strcpy(buffer, "");
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("CRC Coded Dataword: %s\n", buffer);
		}
	}

	return 0;
}

