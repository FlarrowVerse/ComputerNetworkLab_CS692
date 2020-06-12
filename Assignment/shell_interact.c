#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 2048

int main(int argc, char const *argv[])
{
	FILE *fp;
	char *cmd = (char *)calloc(MAX, sizeof(char));
	char *input = (char *)calloc(MAX, sizeof(char));
	char *output = (char *)calloc(MAX, sizeof(char));

	printf("Enter IP address: ");
	fgets(input, MAX, stdin);
	strcpy(cmd, "nslookup ");
	strcat(cmd, input);

	printf("%s: ", cmd);

	fp = popen(cmd, "r");
	fgets(output, MAX, fp);

	printf("%s\n", output);

	printf("Enter host name/url: ");
	fgets(input, MAX, stdin);
	strcpy(cmd, "nslookup ");
	strcat(cmd, input);

	printf("%s: ", cmd);

	fp = popen(cmd, "r");
	fgets(output, MAX, fp);

	printf("%s\n", output);
	return 0;
}