#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define true 1
#define false 0
#define BUFF_SIZE 1024


typedef struct shmseg {
	int cnt;
	int complete;
	char buff[BUFF_SIZE];
} shmseg;

int main(int argc, char *argv[]) {
	int shmid;
	key_t key;

	if ((key = ftok("shm_buffer.txt", 'A')) == -1) {
		printf("File doesn't exist\n");
		return 1;
	}

	shmid = shmget(key, sizeof(shmseg), 0644|IPC_CREAT);
	if (shmid == -1) {
		perror("shmget");
		return 1;
	}

	// Attach to the segment to get a pointer to it.
   	shmseg *shmp = shmat(shmid, NULL, 0);
   	if (shmp == (void *) -1) {
    	perror("Shared memory attach");
      	return 1;
   	}

   	printf("shmseg *seg: %s\n", shmp->buff);

	while (shmp->complete != 1) {
		if (strlen(shmp->buff) > 1) {
			printf("Segment buffer: \n%s\n", shmp->buff);
		}		
		if (shmp->cnt == -1) {
			perror("read");
			return 1;
		}
		printf("Reading process: Shared memory: Read %d bytes\n", shmp->cnt);
		sleep(3);
	}
	printf("Reading Process: Reading Done. Detaching Shared memory.\n");
	if (shmdt(shmp) == -1) {
		perror("shmp");
		return 1;
	}
	printf("Reading process: Complete\n");
	return 0;
}