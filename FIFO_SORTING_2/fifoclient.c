#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "sorter"
#define MAX_LEN 100
#define MIN_LEN 5
#define true 1
#define false 0

int main() {
	int fd, n, i;
	char *buff = (char *)calloc(MAX_LEN, sizeof(char));
	char *buff_out = (char *)calloc(MAX_LEN, sizeof(char));

	printf("Starting FIFO client...............\n");

	fd = open(FIFO_FILE, O_CREAT|O_RDWR);
	
	printf("Enter number of elements: ");
	scanf("%s", buff_out);
	n = atoi(buff_out);
	write(fd, buff_out, strlen(buff_out));

	
	int *arr = (int *)calloc(n, sizeof(int));
	for (i = 0; i < n; i++) {
		printf("Enter a number: ");
		//scanf("%s", buff_out);
		scanf("%d", &arr[i]);
		//write(fd, buff_out, strlen(buff_out));
		//sleep(2);
	}
	//sprintf(buff_out, "%d ", n);
	//printf("Sending array to server......\n");
	/*for (i = 0; i < n; i++) {
		sprintf(buff, "%d ", arr[i]);
		strcat(buff_out, buff);
	}*/
	write(fd, arr, sizeof(int)*n);
	sleep(1);
	printf("Recieved sorted array from server: ");
	int *arr_sorted = (int *)calloc(n, sizeof(int));
	read(fd, arr_sorted, sizeof(int)*n);
	//printf("%s\n", buff);
	for (i = 0; i < n; i++) {		
		printf("%d ", arr_sorted[i]);
	}
	
	printf("\nShutting down client......\n");
	close(fd);	

	return 0;
}
