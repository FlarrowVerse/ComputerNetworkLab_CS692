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

struct sockaddr_in localSock, groupSock;
struct ip_mreq group;
int sd, running;


int main(int argc, char const *argv[]) {

	char *dataBuff = (char *)calloc(MAX, sizeof(char));
	int datalen;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("Opening datagram socket error");
		exit(1);
	} else {
		printf("[+] Opened datagram socket.... \n");
	}

	/* SO_REUSEADDR section*/
	int reuse = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
		perror("Setting SO_REUSEADDR error");
		close(sd);
		exit(1);
	} else {
		printf("[+] Setting SO_REUSEADDR...OK.\n");
	}

	memset((char *)&localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_addr.s_addr = INADDR_ANY;
	localSock.sin_port = htons(4321);
	if (bind(sd, (struct sockaddr*)&localSock, sizeof(localSock))) {
		perror("bind error");
	} else {
		printf("[+] Bind successful....\n");
	}

	memset((char *)&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("239.0.0.1");
	groupSock.sin_port = htons(4321);

	
	group.imr_multiaddr.s_addr = inet_addr("239.0.0.1");
	group.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
		perror("Adding multicast group error");
		close(sd);
		exit(1);
	} else {
		printf("[+] Adding multicast group...OK.\n");
	}

	/* Read from the socket. */
	/*while(1) {
		datalen = read(sd, dataBuff, MAX);
		if (datalen < 0) {
			perror("Reading datagram message error");
			close(sd);
			exit(1);
		} else {
			printf("MULTICAST >>  \"%s\"\n", dataBuff);
		}
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
	int r;
	while (running) {
		// r = read (id, (void *) a, sizeof (a));
		// r = recvfrom(id, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&s, &len);
		r = read(sd, buffer, MAX);
		buffer[r] = 0;
		if (r > 0) {
			if (!strncmp (buffer, "bye", 3))
				running = 0;
			printf ("\nGROUP_CHAT: %s\n", buffer);
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
		// sendto(id, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &s, len);
		if(sendto(sd, buffer, datalen, 0, (struct sockaddr *)&groupSock, sizeof(groupSock)) < 0) {
			perror("sending to group error");
		}
		if (!strncmp (buffer, "bye", 3))
			running = 0;
	}
	pthread_exit (0);
}