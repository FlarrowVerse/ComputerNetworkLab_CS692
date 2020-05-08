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
   int msqid;
   int toend;
   key_t key;
   
   if ((key = ftok("msgq_buffer.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msqid = msgget(key, PERMS)) == -1) { /* connect to the queue */
      perror("msgget");
      exit(1);
   }
   
   printf("Hexadecimal message queue: ready to receive messages.\n");
   buf.mtype = 16;
   while (1) { 
      int bytes = msgrcv(msqid, &buf, 200, buf.mtype, 0);
      if (bytes == -1) {
         perror("msgrcv");
         exit(1);
      }
      char *printBuff = (char *)calloc(bytes, sizeof(char));
      strncpy(printBuff, buf.mtext, bytes);
      printf("Hexadecimal: \"%s\"\n", printBuff);
      free(printBuff);
      toend = strcmp(buf.mtext,"end");
      if (toend == 0)
      break;
      strcpy(buf.mtext, "");
   }
   printf("message queue: done receiving messages.\n");
   system("rm msgq.txt");
   return 0;
}