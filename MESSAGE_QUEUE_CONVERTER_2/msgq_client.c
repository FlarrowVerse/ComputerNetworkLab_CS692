#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "numberConverter.h"

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};

char* decimalToBinary(long n) {
   char *num = (char *)calloc(32, sizeof(char));
   int i, j;
   for (i = 0; i < 32; i++) {
      num[i] = '0';
      num[i] = ((n >> (31-i) & 1) == 1)? '1': '0';
   }
   int flag = false;
   char *bin = (char *)calloc(32, sizeof(char));
   for (i = 0, j = 0; i < 32; i++) {
      if (num[i] == '0' && flag == false) {
         continue;
      } else {
         flag = true;
         bin[j++] = num[i];
      }
   }
   return bin;
}

int main(void) {
   struct my_msgbuf buf;
   int msqid;
   int len;
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
   printf("Enter a number, ^D to quit:\n");
      
   while(fgets(buf.mtext, sizeof(buf.mtext), stdin) != NULL) {
      
      long decimal = atoi(buf.mtext); // original input
      /* writing to 3 packets of message queue */
      // Decimal -> Binary
      sprintf(buf.mtext, "%s", decimalToBinary(decimal));
      len = strlen(buf.mtext);
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      buf.mtype = 2;
      if (msgsnd(msqid, &buf, strlen(buf.mtext), 0) == -1) {
         perror("msgsnd for binary");
      }

      // Decimal -> Octal
      sprintf(buf.mtext, "%lo", decimal);
      len = strlen(buf.mtext);
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      buf.mtype = 8;
      if (msgsnd(msqid, &buf, strlen(buf.mtext), 0) == -1) {
         perror("msgsnd for octal");
      }

      // Decimal -> Hexadecima;
      sprintf(buf.mtext, "%lx", decimal);
      len = strlen(buf.mtext);
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      buf.mtype = 16;
      if (msgsnd(msqid, &buf, strlen(buf.mtext), 0) == -1) {
         perror("msgsnd for hexadecimal");
      }
   }

   strcpy(buf.mtext, "end");
   len = strlen(buf.mtext);
   if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
   perror("msgsnd");
   
   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
   printf("message queue: done sending messages.\n");
   return 0;
}