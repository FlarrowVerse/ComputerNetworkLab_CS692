#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = 9734;
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	else{
		char *bit = (char *)calloc(16, sizeof(char));
		printf("Enter the bit stream : ");
		fflush(stdin);		
		scanf("%[^\n]%*c",bit);
		bit[15]='\0';
		int i=0,c=0;
		while(bit[i]!='\0'){
			if(bit[i]=='1' || bit[i]=='0'){
				i++;				
				continue;
			}
			else{
				printf("Invalid bit stream.");
				c=1;
				i++;				
			}
			
		}
		if(c==0){
			printf("%s\n",bit);
			write(sockfd, bit, strlen(bit));
			printf("Bit stream sent successfully.\n");
			char *output=(char *)calloc(20,sizeof(char));
			read(sockfd, output, 20);
			printf("Stuffed bit stream received from server.\nBit stream after bit stuffing : ");
			printf("%s\n",output);
		}
	}
}
