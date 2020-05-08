/*
 *  Program     :  chatclt.c
 *  Purpose     :  Implements a single server & client chat program using TCP Socket.
 *  Author      :  Dipankar Pal
 *  Last Updated:  19.09.2012
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

void *read_th (void *);
void *write_th (void *);

int id, running = 1;

int
main (int ac, char **av)
{
  struct sockaddr_in c;
  pthread_t pt[2];
  char ip_addr[32];

  if (ac == 1)
    strcpy (ip_addr, "127.0.0.1");
  else
    strcpy (ip_addr, av[1]);
  id = socket (AF_INET, SOCK_STREAM, 0);
  c.sin_family = AF_INET;
  c.sin_addr.s_addr = inet_addr (ip_addr);
  c.sin_port = 1234;
  connect (id, (struct sockaddr *) &c, sizeof (c));
  pthread_create (&pt[0], NULL, read_th, NULL);
  pthread_create (&pt[1], NULL, write_th, NULL);
  pthread_join (pt[0], (void *) 0);
  pthread_join (pt[1], (void *) 0);
  close (id);
  return 0;
}


void *
read_th (void *d)
{
  char a[64];
  int r;
  while (running)
    {
      r = read (id, (void *) a, sizeof (a));
      a[r] = 0;
      if (r > 0)
	{
	  if (!strncmp (a, "bye", 3))
	    running = 0;
	  printf ("\nSERVER: %s\n", a);
	}
    }
  pthread_exit (0);
}


void *
write_th (void *d)
{
  char a[64];
  while (running)
    {
      fprintf (stderr, "> ");
      fgets (a, sizeof (a), stdin);
      write (id, (void *) a, strlen (a) + 1);
      if (!strncmp (a, "bye", 3))
	running = 0;
    }
  pthread_exit (0);
}

