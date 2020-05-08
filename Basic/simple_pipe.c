#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char** createStringArray(int n, int m) {
	char **arr = (char **)calloc(n, sizeof(char *));
	int i;
	for (i = 0; i < n; i++) {
		arr[i] = (char *)calloc(m, sizeof(char));
	}
	return arr;
}

int main() {

	int pipeFD[2];
	int retStatus, row = 2, col = 32, i, j;
	char **writeMsgs = createStringArray(row, col);
	char *readMsg = (char *)calloc(col, sizeof(char));

	for (i = 0; i < row; i++) {
		printf("Enter a message to write: ");
		fgets(writeMsgs[i], col, stdin);
	}

	retStatus = pipe(pipeFD);

	if (retStatus == -1) {
		printf("ERROR: Could not create pipe....\n");
		return 1;
	}

	printf("Writing to the pipe........\n");
	for (i = 0; i < row; i++) {
		printf("Writing message 1.......");
		write(pipeFD[1], writeMsgs[i], col);
		sleep(2);
		printf("Done\n");
	}

	printf("Reading messages from pipe.....\n");
	for (i = 0; i < row; i++) {
		read(pipeFD[0], readMsg, col);
		printf("Message %d: %s\n", i+1, readMsg);
	}


	return 0;
}