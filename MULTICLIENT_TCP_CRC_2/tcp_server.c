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
#define MAXDATA 64

char sub_bit(char bit1, char bit2) {
	return ((bit1 - '0') ^ (bit2 - '0') == 0)? '0': '1';
}

char* multiply(char *str, char bit) {
	char *prod = (char *)calloc(strlen(str), sizeof(char));
	int i;
	for (i = 0; i < strlen(str); i++) {
		prod[i] = (bit == '0')? '0': str[i];
	}
	return prod;
}

char* sub(char *str1, char *str2) {
	char *diff = (char *)calloc(strlen(str1 - 1), sizeof(char));
	int i;
	for (i = 1; i < strlen(str1); i++) {
		diff[i - 1] = sub_bit(str1[i], str2[i]);
	}
	return diff;
}

char* encode_crc(char *bitstream, char *divisor) {
	
	int bitLen = strlen(bitstream), divLen = strlen(divisor), crcLen = divLen - 1;
	int newBitLen = bitLen + crcLen;
	// printf("bitstream is of length: %d; divisor is of length: %d\n", bitLen, divLen);

	char *crc_encoded = (char *)calloc(newBitLen, sizeof(char));
	char *dividend = (char *)calloc(newBitLen, sizeof(char));
	

	strncpy(dividend, bitstream, bitLen);
	memset(dividend + bitLen, '0', crcLen);

	int i = crcLen, j;
	char *rem = (char *)calloc(crcLen, sizeof(char));
	char *curr = (char *)calloc(divLen, sizeof(char));
	strncpy(rem, dividend, crcLen);

	while (i < newBitLen) { // loops through the dividend

		strcpy(curr, rem); // copying the remainder as the subtruend of the next subtraction
		curr[crcLen] = dividend[i]; // bringing down 1 bit from the dividend to attach at the end of the rem
		// printf(" %s\n", curr);		

		char *prod = multiply(divisor, curr[0]); // finding the minuend
		// printf("-%s\n\n", prod);
		strcpy(rem, sub(curr, prod)); // subtracting and putting the result as remainder

		i++; // increment
	}

	strcpy(crc_encoded, bitstream);
	strcat(crc_encoded, rem);

	return crc_encoded;
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

	char *bitstream = (char *)calloc(MAXDATA, sizeof(char));
	char *divisor = (char *)calloc(MAXDATA, sizeof(char));

	long bytes;

	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				bytes = recv(newSocket, buffer, MAXDATA, 0);
				if(strncmp(buffer, "0", 1) == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					
					buffer[bytes] = '\0';				
					strcpy(bitstream, buffer);
					printf("Dataword[%s:%d]: %s\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), buffer);

					bytes = recv(newSocket, buffer, MAXDATA, 0);
					buffer[bytes] = '\0';
					strcpy(divisor, buffer);
					printf("Divisor[%s:%d]: %s\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), buffer);
					
					bytes = strlen(encode_crc(bitstream, divisor));
					printf("Encoded Data: %s\n", encode_crc(bitstream, divisor));
					strcpy(buffer, encode_crc(bitstream, divisor));
					buffer[bytes] = '\0';
					send(newSocket, buffer, bytes, 0);
					
					strcpy(bitstream, "\0");				
					strcpy(divisor, "\0");
					strcpy(buffer, "\0");
				}
			}
		}

	}

	close(newSocket);


	return 0;
}