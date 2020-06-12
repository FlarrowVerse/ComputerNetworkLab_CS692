#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define true 1
#define false 0

int isNumber(char *str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		char ch = str[i];
		if(ch < '0' || ch > '9') {
			return false;
		}
	}
	return true;
}

int isValidIp(char *ip_addr) {
	int i, j, dots = 0;
	ip_addr[strlen(ip_addr) - 1] = '\0';
	printf("%s", ip_addr);
	
	if (ip_addr != NULL) {
		char *ptr = strtok(ip_addr, ".");
		if (ptr == NULL) {
			return false;
		}
		while (ptr) {
			if (!isNumber(ptr)) {
				return false;
			} else {
				int num = atoi(ptr);
				
				if(num < 0 || num > 255) {
					return false;
				} else {
					ptr = strtok(NULL, ".");
					dots++;
				}
			}
		}
		return (dots == 4)? true: false;
	}

	return false;
}

int main() {

	int n, cid, sid, l;
	struct sockaddr_un s, c;
	
	sid = socket(AF_UNIX, SOCK_STREAM, 0);
	s.sun_family = AF_UNIX;
	strcpy(s.sun_path, "FileSocket");
	unlink("FileSocket");
	
	bind(sid, (struct sockaddr*)&s, sizeof(s));
	listen(sid, 1);
	printf("\nServer started. Waiting for connection....\n");
	
	l = sizeof(c);
	cid = accept(sid, (struct sockaddr*)&s, &l);
	printf("Connection Accepted. Obtaining Inputs....\n");
	
	char *ip_addr = (char *)calloc(16, sizeof(char));
	read(cid, ip_addr, 16);
	int result = isValidIp(ip_addr);
	char *out = (char *)calloc(1, sizeof(char));
	if(result == true) {
		out[0] = 'Y';
	} else {
		out[0] = 'N';
	}
	printf("\nSending back reply: %c\n", out[0]);
	write(cid, out, 1);
	printf("Shutting down server......\n");

	return 0;
}
