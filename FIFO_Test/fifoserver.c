#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "/fifo_files/sorter"
#define MAX_LEN 100
#define MIN_LEN 20
#define true 1
#define false 0

void sort(int[]);

int main() {
	int fd, n, i;

	// creating the FIFO special file
	int retVal = mkfifo(FIFO_FILE, S_IFIFO|0640);
	if (retVal == -1) {
		printf("ERROR: Failed to create FIFO...\n");
		return 1;
	}

	fd = open(FIFO_FILE, O_RDWR);
	fscanf(fd, "%d", &n);
	int *arr = (int *)calloc(n, sizeof(int));

	printf("Recieved array from client: ");
	for (i = 0; i < n; i++) {
		fscanf(fd, "%d", &arr[i]);
		printf("%d\t", arr[i]);
	}
	printf("\nSorting array.........\n");
	sort(arr);
	printf("Sending sorted array.........\n");
	printf("Shutting down server......\n");
	close(fd);
	
	
	return 0;
}