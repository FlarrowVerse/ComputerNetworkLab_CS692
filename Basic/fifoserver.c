#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "MYFIFO"
#define MAX_LEN 100
#define MIN_LEN 20
#define true 1
#define false 0

int main() {
	int fd;
	char *readBuffer = (char *)calloc(MAX_LEN, sizeof(char)), *end = (char *)calloc(MIN_LEN, sizeof(char));
	int to_end, read_bytes;

	// creating the FIFO special file
	int retVal = mkfifo(FIFO_FILE, 0640);
	if (retVal == -1) {
		printf("ERROR: Failed to create FIFO...\n");
		return 1;
	}

	strcpy(end, "end");
	while (true) {
		fd = open(FIFO_FILE, O_RDONLY);
		read_bytes = read(fd, readBuffer, MAX_LEN);
		readBuffer[read_bytes] = '\0';
		printf("Message from Client: %s\n", readBuffer);

		to_end = strcmp(readBuffer, end);
		if (to_end == 0) {
			printf("Shutting down server......\n");
			close(fd);
			break;
		}
	}

	return 0;
}