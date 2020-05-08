#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	fork();

	printf("fork() system call has been invoked\n");
	
	return 0;
}