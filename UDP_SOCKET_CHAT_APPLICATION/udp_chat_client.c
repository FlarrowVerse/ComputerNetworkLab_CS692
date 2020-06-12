#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <signal.h>

#define SERVER_PORT 8080
#define CLIENT_PORT 8081
#define MAXLINE 1024

int main() {

	int sockfd, flag = 0;
	char buffer[MAXLINE];
	
	struct sockaddr_in servaddr, clientaddr;
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        close(sockfd);
        exit(EXIT_FAILURE); 
    }
    printf("[+] Socket created successfully....\n");

	// setting the server address/port commbo
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(SERVER_PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

    // setting the client address/port commbo
	memset(&clientaddr, 0, sizeof(clientaddr));
	clientaddr.sin_family = AF_INET; 
    clientaddr.sin_port = htons(CLIENT_PORT); 
    clientaddr.sin_addr.s_addr = INADDR_ANY; 

    if ( bind(sockfd, (const struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0 ) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    printf("[+] Bind successful....\n");    

	int n, len = sizeof(servaddr), err;

	int pid = fork();

	if (pid == 0) {
		while (flag != 1) {
			memset(buffer, 0, sizeof(buffer));
			// printf("CLIENT> ");
			fgets(buffer, MAXLINE, stdin);
			buffer[strlen(buffer)] = '\0';

			int buff_len = strlen(buffer);			
			err = sendto(sockfd, buffer, buff_len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, len); 
			if (err == -1) {
				perror("Error in sending message.....\n");
				exit(EXIT_FAILURE);
			}
			printf("ME> %s", buffer);
			if (strncmp(buffer, "end", 3) == 0) {
				flag = 1;
                break;
            }			
		}
		// printf("Closing Application....\n");
		// sleep(2);
		// close(sockfd);
		// unlink("8081");
		printf("Closing child...OR Input unit.\n");
		kill(getppid(), SIGABRT);
	} else {
		while (flag != 1) {
			memset(buffer, 0, sizeof(buffer));
			n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
			if (n == -1) {
				perror("Error in recieving message.....\n");
				exit(EXIT_FAILURE);
			}
			if(strncmp("end", buffer, 3) == 0) {
                // printf("[+] Shutting down server........\n");
                flag = 1;
                break;
            }
			//buffer[strlen(buffer)-1] = '\0';
			printf("SERVER> %s\n", buffer);
		
		}
		printf("Closing Application....\n");
		kill(pid, SIGABRT);
		sleep(2);
		close(sockfd);
		unlink("8081");
	}
	
	// printf("Closing Application....\n");
	// sleep(2);
	// close(sockfd);
	// unlink("8081");
	return 0;
}
