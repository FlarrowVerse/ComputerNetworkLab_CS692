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

	int pipefd_in[2], pipefd_out[2];
	int inpipe_status, outpipe_status, row = 2, col = 32, i, pid, waitStatus;
	char **writeMsgs = createStringArray(row, col);
	char *readMsg = (char *)calloc(col, sizeof(char));

	/*for (i = 0; i < row; i++) {
		printf("Enter a message to write: ");
		fgets(writeMsgs[i], col, stdin);
	}*/

	inpipe_status = pipe(pipefd_in); // input pipe
	if (inpipe_status == -1) {
		printf("ERROR: Could not create input pipe....\n");
		return 1;
	}

	outpipe_status = pipe(pipefd_out); // output pipe
	if (outpipe_status == -1) {
		printf("ERROR: Could not create output pipe....\n");
		return 1;
	}

	pid = fork();

	if (pid == 0) {
		// child process
		
		//printf("Reading messages from pipe.....Process Info:: PID: %d; PPID: %d\n", getpid(), getppid());
		printf("Enter a message to deliver from %d: ", getpid());
		fgets(writeMsgs[0], col, stdin);
		write(pipefd_in[1], writeMsgs[0], col);
		read(pipefd_in[0], readMsg, col);
		printf("Message for %d: %s", getpid(), readMsg);
		
	} else {
		// parent process
		//printf("Writing to the pipe........Process Info:: PID: %d; PPID: %d\n", getpid(), getppid());
		wait(&waitStatus);
		printf("Enter a message to deliver from %d: ", getpid());
		fgets(writeMsgs[1], col, stdin);
		write(pipefd_out[1], writeMsgs[1], col);
		read(pipefd_out[0], readMsg, col);
		printf("Message for %d: %s", getpid(), readMsg);
	}

	return 0;
}