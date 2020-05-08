#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
// #include "studentSorter.h"

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
   msgsnd(msqid, &buf, strlen(buf.mtext), 0);
   
   int n = atoi(buf.mtext);
   int *arr = (int *)calloc(n, sizeof(int));
   for (i = 0; i < n; i++) {
   		scanf("%d", &arr[i]);
   }

   msgsnd(msqid, arr, n * sizeof(int), 0);
   
   printf("Done sending messages\n");
   return 0;
}