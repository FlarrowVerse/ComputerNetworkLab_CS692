#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include "./Student/studentSorter.h"

#define true 1
#define false 0
#define PERMS 0644

struct my_msgbuf {
   long mtype;
   char mtext[200];
};

int main(void) {
	struct my_msgbuf buf;
	int msqid, len, i, j;
	key_t key;
   
	if ((key = ftok("msgq_buffer.txt", 'B')) == -1) {
   		perror("ftok");
    	exit(1);
	}
   
	if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
   		perror("msgget");
    	exit(1);
	}
	printf("message queue: ready to send messages.\n");
	printf("Enter number of records:\n");
	char *input = (char *)calloc(32, sizeof(char));
	fgets(buf.mtext, 32, stdin);
	buf.mtype = 1;
	msgsnd(msqid, &buf, sizeof(buf.mtext), 0); // sending number of records only

	buf.mtype = 2;
	msgsnd(msqid, &buf, sizeof(buf.mtext), 0); // sending number of records only
   
	int n = atoi(buf.mtext); // number of records
	student **records = createNewList(n);

	for (i = 0; i < n; i++) {
		strcpy(buf.mtext, "\n");
		
		char *buff = toString(records[i]);
		
		strcpy(buf.mtext, buff);

		buf.mtype = 1;
		msgsnd(msqid, &buf, sizeof(buf.mtext), 0);
		buf.mtype = 2;
		msgsnd(msqid, &buf, sizeof(buf.mtext), 0);

		free(buff);
		free(records[i]);
		sleep(1);
	}

	printf("Sorted according to Roll No.: \n");
	for (i = 0; i < n; i++) {
		
    	msgrcv(msqid, &buf, 300, 1, 0);
    	records[i] = parseStudent(buf.mtext);
    	printf("%s\n", toString(records[i]));
    	sleep(1);
   	}

   	printf("Sorted according to Name: \n");
	for (i = 0; i < n; i++) {
		
    	msgrcv(msqid, &buf, 300, 2, 0);
    	records[i] = parseStudent(buf.mtext);
    	printf("%s\n", toString(records[i]));
    	sleep(1);
   	}

	
	printf("Done sending messages\n");
	return 0;
}