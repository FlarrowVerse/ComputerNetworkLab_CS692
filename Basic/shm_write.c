#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define BUFF_SIZE 1024
#define true 1
#define false 0

typedef struct shmseg {
	int cnt;
	int complete;
	char buff[BUFF_SIZE];
} shmseg;

int fill_buffer(char *, int);

int main(int argc, char *argv[]) {
	int shmid, numTimes;
	shmseg *shmp;
	char *buffptr;
	int spaceAvailable;
	key_t key;

	if ((key = ftok("shm_buffer.txt", 'A')) == -1) {
		printf("File doesn't exist\n");
		return 1;
	}

	shmid = shmget(key, sizeof(shmseg), 0644|IPC_CREAT);
	if (shmid == -1) {
		perror("Shared memory");
		return 1;
	}

	// attach to the segment to get a pointer to it
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1) {
		perror("Shared memory attach");
		return 1;
	}

	/* Transfer data from buffer to shared memory segment */
	buffptr = shmp->buff;
	spaceAvailable = BUFF_SIZE;

	for (numTimes = 0; numTimes < 3; numTimes++) {
		shmp->cnt = fill_buffer(buffptr, spaceAvailable);
		shmp->complete = false;
		printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp->cnt);
		buffptr = shmp->buff;
		spaceAvailable = BUFF_SIZE;
		sleep(3);
	}

	printf("Writing process done. Wrote %d times.\n", numTimes);
	shmp->complete = 1;

	if (shmdt(shmp) == -1) {
		perror("shmdt");
		return 1;
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("shmctl");
		return 1;
	}

	printf("Writing process done.\n");
	return 0;
}

int fill_buffer(char *buffptr, int size) {
	char filler = '-';
	int filledCount = 0;

	char *input = (char *)calloc(size, sizeof(char));
	printf("Enter a message: ");
	fgets(input, size, stdin);

	filledCount = strlen(input);
	memcpy(buffptr, input, strlen(input));

	return filledCount;
}