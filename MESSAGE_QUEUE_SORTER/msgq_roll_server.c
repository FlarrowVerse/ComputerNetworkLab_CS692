#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>

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
   msgrcv(msqid, &buf, 200, 0, 0);
   int n = atoi(buf.mtext);
   printf("received: %d\n", n);

   // int *arr = (int *)calloc(n, sizeof(int));
   int *arr = NULL;
   msgrcv(msqid, arr, n * sizeof(int), 0, 0);
   for (i = 0; i < n; i++) {
      printf("%d ", arr[i]);
   }
   printf("\ndone receiving...\n");
   system("rm msgq.txt");
   return 0;
}