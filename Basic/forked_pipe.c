#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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
	int retStatus, row = 2, col = 32, i, pid, waitStatus;
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

	pid = fork();

	if (pid == 0) {
		// child process
		waitpid(getpid(), &waitStatus, 0);
		printf("Reading messages from pipe.....Process Info:: PID: %d; PPID: %d\n", getpid(), getppid());
		for (i = 0; i < row; i++) {
			read(pipeFD[0], readMsg, col);
			printf("Message %d: %s", i+1, readMsg);
		}
	} else {
		// parent process
		printf("Writing to the pipe........Process Info:: PID: %d; PPID: %d\n", getpid(), getppid());
		for (i = 0; i < row; i++) {
			printf("Writing message %d.......", i+1);
			write(pipeFD[1], writeMsgs[i], col);
			printf("Done\n");
		}
	}

	return 0;
}