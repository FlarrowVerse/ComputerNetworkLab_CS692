#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define true 1
#define false 0

int compare(const void *a, const void *b) {
	return *((int *)a) - *((int *)b);
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

	//void *ptr = NULL;
	read(cid, &n, sizeof(int *));
	//n = *((int *)ptr);
	printf("Recieved Size: %d\n", n);

	int *arr = (int *)calloc(n, sizeof(int));

	read(cid, arr, n * sizeof(int *));
	//arr = (int *)ptr;
	printf("Recieved Array: ");
	int i;
	for (i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}

	qsort(arr, n, sizeof(int), compare);

	printf("\nSorted array: ");
	for (i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}

	printf("\nSending back sorted array.......\n");
	write(cid, arr, n * sizeof(int *));
	
	// char *ip_addr = (char *)calloc(16, sizeof(char));
	// read(cid, ip_addr, 16);
	// int result = isValidIp(ip_addr);
	// char *out = (char *)calloc(1, sizeof(char));
	// if(result == true) {
	// 	out[0] = 'Y';
	// } else {
	// 	out[0] = 'N';
	// }
	// printf("\nSending back reply: %c\n", out[0]);
	// write(cid, out, 1);

	printf("Shutting down server......\n");

	return 0;
}
