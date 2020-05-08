#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

#define MAXLINE 1024

void *read_th (void *);
void *write_th (void *);

int id, running = 1;
struct sockaddr_in c, s;

int main (int ac, char **av)
{
  
  pthread_t pt[2];
  char ip_addr[32];

  if (ac == 1)
    strcpy (ip_addr, "127.0.0.1");
  else
    strcpy (ip_addr, av[1]);
  id = socket (AF_INET, SOCK_DGRAM, 0);
  c.sin_family = AF_INET;
  c.sin_addr.s_addr = inet_addr (ip_addr);
  c.sin_port = htons(8081);
  // connect (id, (struct sockaddr *) &c, sizeof (c));
  int k = bind (id, (struct sockaddr *) &c, sizeof (c));

  s.sin_family = AF_INET;
  s.sin_addr.s_addr = inet_addr (ip_addr);
  s.sin_port = htons(8080);
  pthread_create (&pt[0], NULL, read_th, NULL);
  pthread_create (&pt[1], NULL, write_th, NULL);
  pthread_join (pt[0], (void *) 0);
  pthread_join (pt[1], (void *) 0);
  close (id);
  return 0;
}


void *read_th (void *d) {
  char *buffer = (char *)calloc(MAXLINE, sizeof(char));
  int r, len = sizeof(s);
  while (running)
    {
      // r = read (id, (void *) a, sizeof (a));
      r = recvfrom(id, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&s, &len);
      buffer[r] = 0;
      if (r > 0) {
        if (!strncmp (buffer, "bye", 3))
          running = 0;
	      printf ("\nSERVER: %s\n", buffer);
      }
    }
  pthread_exit (0);
}


void *write_th (void *d) {
  // char a[64];
  char *buffer = (char *)calloc(MAXLINE, sizeof(char));
  int len = sizeof(s);
  while (running)
    {
      fprintf (stderr, "> ");
      fgets (buffer, MAXLINE, stdin);
      // write (id, (void *) a, strlen (a) + 1);
      sendto(id, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &s, len);
      if (!strncmp (buffer, "bye", 3))
      	running = 0;
    }
  pthread_exit (0);
}

