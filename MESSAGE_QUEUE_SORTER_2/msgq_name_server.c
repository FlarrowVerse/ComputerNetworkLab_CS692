#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include "./Student/studentSorter.h"

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};

int main(void) {
	struct my_msgbuf buf;
	int msqid, toend, i;
	key_t key;
   
	if ((key = ftok("msgq_buffer.txt", 'B')) == -1) {
    	perror("ftok");
    	exit(1);
	}
   
	if ((msqid = msgget(key, PERMS)) == -1) { /* connect to the queue */
    	perror("msgget");
    	exit(1);
	}
	printf("message queue: ready to receive messages.\n");
	msgrcv(msqid, &buf, sizeof(buf.mtext), 2, 0);
	int n = atoi(buf.mtext);
	printf("received: %d\n", n);

	student **records = (student **)calloc(n, sizeof(student *));
   
	for (i = 0; i < n; i++) {
    	msgrcv(msqid, &buf, sizeof(buf.mtext), 2, 0);
    	records[i] = parseStudent(buf.mtext);
    	printf("Recieved: %s\n", buf.mtext);
    	
    	sleep(1);
   	}

   	sortStudents(records, n, "name");

	// for (i = 0; i < n; i++) {
 //    	char *buff = toString(records[i]);
 //    	printf("%s\n", buff);
 //    	free(buff);
	// }

	for (i = 0; i < n; i++) {
		char *buff = toString(records[i]);
		strcpy(buf.mtext, buff);
		buf.mtype = 2;
		msgsnd(msqid, &buf, sizeof(buf.mtext), 0);
		free(buff);
		free(records[i]);
		sleep(1);
	}

	printf("\ndone receiving...\n");
   
	return 0;
}