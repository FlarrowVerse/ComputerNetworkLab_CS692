#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {

	int sid, i, N;
	sid = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un s;
	s.sun_family = AF_UNIX;
	strcpy(s.sun_path, "FileSocket");
	connect(sid, (struct sockaddr*)&s, sizeof(s));
	printf("Connected to Server.\n");
	
	printf("Enter the IPV4 Address: ");
	char *ip_addr = (char *)calloc(16, sizeof(char));
	fgets(ip_addr, 16, stdin);	
	printf("Sending IPV4 Address to be validated.....\n");
	
	write(sid, ip_addr, strlen(ip_addr)); // writing to the file socket
	
	char server_output;
	read(sid, &server_output, 1);
	
	if (server_output == 'Y') {
		printf("Given IP is a valid IPV4 Address.\n");
	} else {
		printf("Given IP is not a valid IPV4 Address.\n");
	}
	
	printf("Shutting down client connection......\n");

	return 0;
}
