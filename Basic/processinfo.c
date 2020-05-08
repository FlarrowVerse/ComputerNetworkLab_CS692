#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

	printf("Program to know current process informations......\n");

	int curr_pid = getpid();
	int parent_pid = getppid();

	printf("Process ID of ./processinfo: %d\n", curr_pid);
	printf("Process ID of parent of ./processinfo: %d\n", parent_pid);

	printf("Verifying result by matching with system call.....$ top\n");
	system("top -b | grep processinfo");
	
	return 0;
}