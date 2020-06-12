#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT 8080
#define MAXLINE 1024

int main() {

	int sockfd;
	char buffer[MAXLINE];
	
	struct sockaddr_in servaddr;
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        close(sockfd);
        exit(EXIT_FAILURE); 
    }
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

	int n, len = sizeof(servaddr), err;

	while (1) {
		memset(buffer, 0, sizeof(buffer));
		printf("Enter the dataword: ");
		scanf("%s", buffer);

		int buff_len = strlen(buffer);
		if(strncmp("end", buffer, 3) == 0) {
			err = sendto(sockfd, buffer, buff_len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, len); 
			if (err == -1) {
				perror("Error in sending message.....\n");
				exit(EXIT_FAILURE);
			}
			break;
		}

		int flag = 1, i;
		for (i = 0; i < buff_len; i++) {
			if (buffer[i] != '1' && buffer[i] != '0') {
				printf("Dataword is invalid.....\n");
				flag = 0;
				break;
			}
		}

		if (flag == 1) {
			err = sendto(sockfd, buffer, buff_len, MSG_CONFIRM, (const struct sockaddr *) &servaddr, len); 
			if (err == -1) {
				perror("Error in sending message.....\n");
				exit(EXIT_FAILURE);
			}
			printf("Dataword sent to server........\n");

			memset(buffer, 0, sizeof(buffer));
			n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
			if (n == -1) {
				perror("Error in recieving message.....\n");
				exit(EXIT_FAILURE);
			}
			buffer[n] = '\0';
			printf("Codeword recieved from server: %s\n", buffer);
		}
	}
	printf("Closing Application....\n");
	sleep(2);
	close(sockfd);
	return 0;
}