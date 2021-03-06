#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define SERVER_PORT 8080
#define CLIENT_PORT 8081
#define MAXLINE 1024
#define MAX 1024

int main() {

	int sockfd;
	char buffer[MAXLINE];
	
	struct sockaddr_in servaddr, cliaddr;
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");		
		exit(EXIT_FAILURE);
	}
	printf("[+] Socket created.....\n");

	// setting the server address/port commbo
	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(SERVER_PORT);

	
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    printf("[+] Bind successful.....\n");

    memset(&cliaddr, 0, sizeof(cliaddr)); 
    cliaddr.sin_family    = AF_INET; // IPv4 
    cliaddr.sin_addr.s_addr = INADDR_ANY; 
    cliaddr.sin_port = htons(CLIENT_PORT);
    
    int len, n, err; 
    len = sizeof(cliaddr);  //len is value/resuslt  

    printf("[+] Server ready to communicate.....\n");

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "Hello, welcome to the Chat Server....");
    buffer[strlen(buffer)] = '\0';

    err = sendto(sockfd, buffer, strlen(buffer),MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    if(err == -1) {
    	perror("[-] Error in sending reply.....\n");
    	exit(EXIT_FAILURE);
    }

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
    	// printf("[+] Recieved successfully......\n");
    	// printf("\tDataword Recieved: %s\n", buffer);

    	// char *codedData = hamming_code(buffer);
    	// printf("\tSending coded Data \"%s\" to client.......\n", codedData);
    	printf("%s <CLIENT\n", buffer);
    	memset(buffer, 0, sizeof(buffer));

    	printf("SERVER> ");
    	fgets(buffer, MAXLINE, stdin);
    	err = sendto(sockfd, buffer, strlen(buffer),MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    	if(err == -1) {
    		perror("[-] Error in sending reply.....\n");
    		exit(EXIT_FAILURE);
    	}
    	// printf("\tCode word sent successfully\n");
    }
    printf("[+] Shutting down server\n");  
    close(sockfd);
    unlink("8081");
    unlink("8080");
    return 0; 	
}