#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int cid, server_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address,s,c;
	struct sockaddr_in client_address;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = 9734;
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);

	printf("Server started.\n");
	int l=sizeof(c);
	cid = accept(server_sockfd, (struct sockaddr*)&s, &l);
	char *bit_stream = (char *)calloc(16, sizeof(char));
	char *out= (char *)calloc(20, sizeof(char));
	read(cid, bit_stream, 16);
	int i=0,j=0, count=0,loc=-99;
	while(bit_stream[i]!='\0'){
		out[j]=bit_stream[i];
		if(bit_stream[i]=='0')
			count=0;		
		if(bit_stream[i]=='1'){			
			count++;
			if(count==5)
				loc=i;
		}
		if(loc!=-99){
			out[loc+1]='0';
			j=loc+1;
			count=0;
			loc=-99;
		}
		j++;
		i++;
	}
	printf("%s\n",out);
	printf("Sending back to client.\n");
	write(cid,out,j);
	printf("Shutting down server.");	
}
