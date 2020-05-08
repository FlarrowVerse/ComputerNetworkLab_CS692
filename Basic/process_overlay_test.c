#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
	char *execName = (char *)calloc(100, sizeof(char));

	printf("Enter filename to be executed: ");
	fscanf(stdin, "%s", execName);
	
	execl(execName, execName, (char *)NULL);

}