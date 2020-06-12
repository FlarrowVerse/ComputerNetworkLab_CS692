#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {

	int sid, i, N;
	sid = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un s;
	s.sun_family = AF_UNIX;
	strcpy(s.sun_path, "FileSocket");
	connect(sid, (struct sockaddr*)&s, sizeof(s));
	printf("Connected to Server.\n");
	
	// printf("Enter the IPV4 Address: ");
	// char *ip_addr = (char *)calloc(16, sizeof(char));
	// fgets(ip_addr, 16, stdin);	
	// printf("Sending IPV4 Address to be validated.....\n");

	int n;
	printf("Enter the total size of array: ");
	scanf("%d", &n);
	int *arr = (int *)calloc(n, sizeof(int)); // dynamic allocation

	printf("Sending array size.....\n");
	write(sid, &n, sizeof(int *)); // writing to the file socket

	for (i = 0; i < n; i++) {
		printf("Enter arr[%d]: ", i);
		scanf("%d", &arr[i]);
	}
	printf("Sending array......\n");
	write(sid, arr, n * sizeof(int *));

	printf("Reading array.......\n");
	read(sid, arr, n * sizeof(int *));

	printf("Sorted array:: \n");
	for (i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	
	// char server_output;
	// read(sid, &server_output, 1);
	
	// if (server_output == 'Y') {
	// 	printf("Given IP is a valid IPV4 Address.\n");
	// } else {
	// 	printf("Given IP is not a valid IPV4 Address.\n");
	// }
	
	printf("\nShutting down client connection......\n");

	return 0;
}
