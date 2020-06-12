#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 1024

void *read_th (void *);
void *write_th (void *);

struct sockaddr_in groupSock, s;
int sd, running;

int main(int argc, char const *argv[]) {

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("Opening datagram socket error");
		exit(1);
	} else {
		printf("[+] Opened datagram socket.... \n");
	}

	memset((char *)&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("239.0.0.1");
	groupSock.sin_port = htons(4321);

	/*char *dataBuff = (char *)calloc(MAX, sizeof(char));
	int datalen;
	while(1) {		
		printf("Enter a message to broadcast: ");
		fgets(dataBuff, MAX, stdin);
		datalen = strlen(dataBuff);
		dataBuff[datalen-1] = '\0';

		if (sendto(sd, dataBuff, datalen, 0, (struct sockaddr *)&groupSock, sizeof(groupSock)) < 0) {
			perror("sending dgram error");
		} else {
			printf("[+] Sent the message to datagram message....\n");
		}
		sleep(2);
		if (strncmp("bye", dataBuff, 3) == 0) {
			close(sd);
			printf("broadcast closed!\n");
			break;
		}
	}*/

	pthread_t pt[2];
	running = 1;
	pthread_create(&pt[0], NULL, read_th, NULL);
	pthread_create(&pt[1], NULL, write_th, NULL);

	pthread_join (pt[0], (void *) 0);
  	pthread_join (pt[1], (void *) 0);

  	close(sd);
	

	return 0;
}

void *read_th (void *d) {
	char *buffer = (char *)calloc(MAX, sizeof(char));
	int r, len = sizeof(&s);
	while (running) {
		// r = read (id, (void *) a, sizeof (a));
		// r = recvfrom(sd, (char *)buffer, MAX, MSG_WAITALL, (struct sockaddr *)&s, &len);
		r = read(sd, buffer, MAX);
		buffer[r] = 0;
		if (r > 0) {
			if (!strncmp (buffer, "bye", 3))
				running = 0;
			printf ("\nCLIENT: %s\n", buffer);
		}
	}
	pthread_exit (0);
}

void *write_th (void *d) {
	// char a[64];
	char *buffer = (char *)calloc(MAX, sizeof(char));
	while (running) {
		fprintf (stderr, "> ");
		fgets (buffer, MAX, stdin);
		int datalen = strlen(buffer);
		// write (id, (void *) a, strlen (a) + 1);
		//sendto(id, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &c, len);
		if(sendto(sd, buffer, datalen, 0, (struct sockaddr *)&groupSock, sizeof(groupSock)) < 0) {
			perror("sending to group error");
		}
		if (!strncmp (buffer, "bye", 3))
			running = 0;
	}
	pthread_exit (0);
}