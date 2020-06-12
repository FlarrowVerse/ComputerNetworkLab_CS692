#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 8080
#define MAXLINE 1024
#define MAX 1024

int* getIndices(int pos, int max_bits, int *len) {
	int i, count = 0;
	for (i = 1; i <= max_bits; i++) {
		if ((pos & i) == pos && pos != i) {
			count++;
		}
	}
	int *nums = (int *)calloc(count, sizeof(int)), index = 0;
	*len = count;
	for (i = 1; i <= max_bits; i++) {
		if ((pos & i) == pos && pos != i) {
			nums[index] = i;
			index++;
		}
	}
	return nums;
}

int getRedundantBits(int dataBits) {
	int redBits = 0;
	while((1 << redBits) < (dataBits + redBits + 1)) {
		redBits++;
	}
	return redBits;
}

int getParityBit(int *nums, int len, char *bitStream) {
	int parity = 0, i, total = strlen(bitStream);
	for (i = 0; i < len; i++) {
		if (bitStream[total - nums[i]] == '1') {
			parity = (parity == 0)? 1: 0;
		}
	}
	return parity;
}

char* hamming_code(char *data) {
	// printf("%lu %c %s\n", strlen(data), data[strlen(data) - 1], data);
	int dataBits = strlen(data), redBits = getRedundantBits(dataBits), i = 0;
	int total = dataBits + redBits;
	char *codedData = (char *)calloc(total, sizeof(char));
	for (i = 0; i < redBits; i++) {
		int currPos = 1 << i;
		codedData[total - currPos] = 'R';
	}	
	int k = 0;
	for (i = 0; i < total; i++) {
		if (codedData[i] != 'R') {
			codedData[i] = data[k];
			k++;
		}
	}
	for (i = 0; i < redBits; i++) {
		int currPos = 1 << i, len = 0;
		int *arr = getIndices(currPos, total, &len);
		int res = getParityBit(arr, len, codedData);
		codedData[total - currPos] = (res == 1)? '1': '0';
	}
	printf("Hamming Coded Data: %s\n", codedData);
	return codedData;
}

int main() {

	int sockfd;
	char buffer[MAXLINE];
	
	struct sockaddr_in servaddr, cliaddr;
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");		
		exit(EXIT_FAILURE);
	}
	printf("[+] Socket created.....\n");
	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT);

	
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    printf("[+] Bind successfull.....\n");
    memset(&cliaddr, 0, sizeof(cliaddr)); 
    
    // TODO: 6 Directly use recvfrom() api to receive data from client  
    int len, n, err; 
    len = sizeof(cliaddr);  //len is value/resuslt  

    printf("[+] Server ready to recieve dataword.....\n");

    while (1) {
    	memset(buffer, 0, sizeof(buffer));
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

    	if (n == -1) {
    		perror("[-] Error in recieve....\n");
    		exit(EXIT_FAILURE);    		
    	}
    	else if(strncmp("end", buffer, 3) == 0) {
    		// printf("[+] Shutting down server........\n");
    		break;
    	}

    	//buffer[n] = '\0';
    	printf("[+] Recieved successfully......\n");
    	printf("\tDataword Recieved: %s\n", buffer);

    	char *codedData = hamming_code(buffer);
    	printf("\tSending coded Data \"%s\" to client.......\n", codedData);

    	err = sendto(sockfd, codedData, strlen(codedData),MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    	if(err == -1) {
    		perror("[-] Error in sending reply.....\n");
    		exit(EXIT_FAILURE);
    	}
    	printf("\tCode word sent successfully\n");
    }
    printf("[+] Shutting down server\n");  
    close(sockfd);
    unlink("8080");
    return 0; 	
}