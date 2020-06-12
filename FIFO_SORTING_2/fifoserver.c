#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "sorter"
#define MAX_LEN 100
#define MIN_LEN 20
#define true 1
#define false 0

void sort(int [], int);
int compare(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

int main() {
	int fd, i;
	char *buff = (char *)calloc(MAX_LEN, sizeof(char));
	char *buff_out = (char *)calloc(MAX_LEN, sizeof(char));

	// creating the FIFO special file
	int retVal = mkfifo(FIFO_FILE, S_IFIFO|0640);
	if (retVal == -1) {
		printf("ERROR: Failed to create FIFO...\n");
		return 1;
	}
	
	fd = open(FIFO_FILE, O_RDWR);
	read(fd, buff, MAX_LEN);
	int n = atoi(buff);
	int *arr = (int *)calloc(MAX_LEN, sizeof(int));
	
	//sscanf(buff, "%d %d %d %d", &n, &arr[0], &arr[1], &arr[2]);	

	//printf("Recieved array from client: %s\n", buff);
	printf("n=%d\n", n);
	
	read(fd, arr, sizeof(int)*n);
	
	for (i = 0; i < n; i++) {
		
		//printf("Recieved: %s\n", buff);
		printf("%d ", arr[i]);
	}
	printf("\nSorting array.........\n");
	sort(arr, n);
	printf("Sending sorted array.........\n");
	for (i = 0; i < n; i++) {		
		printf("%d ", arr[i]);
	}
	
	write(fd, arr, sizeof(int)*n);

	printf("\nShutting down server......\n");
	close(fd);
		
	return 0;
}

void sort(int arr[], int n) {
	qsort(arr, n, sizeof(int), compare);
}
