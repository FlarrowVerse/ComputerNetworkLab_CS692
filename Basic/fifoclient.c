#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "MYFIFO"
#define MAX_LEN 100
#define MIN_LEN 5
#define true 1
#define false 0

int main() {
	int fd;
	char *writeBuffer = (char *)calloc(MAX_LEN, sizeof(char)), *end = (char *)calloc(MIN_LEN, sizeof(char));
	int to_end, bytes;

	printf("Starting FIFO client............... Usage: Unlimited;(type end to exit the client):");
	strcpy(end, "end");

	fd = open(FIFO_FILE, O_CREAT|O_WRONLY);

	while (true) {
		printf("Enter a message to send to server: ");
		fgets(writeBuffer, MAX_LEN, stdin);
		bytes = strlen(writeBuffer);
		writeBuffer[bytes-1] = '\0';

		write(fd, writeBuffer, strlen(writeBuffer));

		to_end = strcmp(writeBuffer, end);
		if (to_end == 0) {			
			printf("Shutting down client......\n");
			close(fd);
			break;
		}
	}

	return 0;
}