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

void check_hostname(int hostname_status) {
	if (hostname_status == -1) {
		perror("gethostname");
		exit(0);
	}
}

void check_host_entry(struct hostent *hostentry) {
	if (hostentry == NULL) {
		printf("Host undetermined.\n");
		perror("host entry check:");
		exit(0);
	}	
}

void format_ip(char *ip_buffer) {
	if (ip_buffer == NULL) {
		perror("inet_ntoa");
		exit(0);
	}
}

int main(int argc, char const *argv[]) {

	//char *hostname = (char *)calloc(256, sizeof(char));
	//char *IP = (char *)calloc(16, sizeof(char));
	struct hostent *host_entry;// = (struct hostent *)malloc(sizeof(struct hostent));
	struct in_addr *addr = (struct in_addr *)malloc(sizeof(struct in_addr));
	// struct in_addr addr;

	char *input = (char *)calloc(MAX, sizeof(char));
	char *output = (char *)calloc(MAX, sizeof(char));

	printf("Enter IP address: ");
	fgets(input, MAX, stdin);
	input[strlen(input)-1] = '\0';

	inet_aton(input, addr);
	host_entry = gethostbyaddr(addr, sizeof(struct in_addr), AF_INET);
	check_host_entry(host_entry);
	
	sprintf(output, 
		"Hostname: %s\n", 
		host_entry->h_name);
	printf("Hostname of ip(%s): %s\n", input, output);

	printf("Enter host name/url: ");
	fgets(input, MAX, stdin);
	input[strlen(input)-1] = '\0';

	host_entry = gethostbyname(input);
	check_host_entry(host_entry);
	
	sprintf(output, 
		"Hostname: %s\nHost IP: %s\n", 
		host_entry->h_name, inet_ntoa(*(struct in_addr *)host_entry->h_addr));
	printf("IP address of hostname(%s): %s\n", input, output);

	return 0;
}