/*
 *  Program     :  chatsrv.c
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

int cid, running = 1;

int
main (int ac, char **av)
{
  int l, k, sid;
  struct sockaddr_in s, c;
  pthread_t pt[2];
  char ip_addr[32];

  if (ac == 1)
    strcpy (ip_addr, "127.0.0.1");
  else
    strcpy (ip_addr, av[1]);
  sid = socket (AF_INET, SOCK_STREAM, 0);
  s.sin_family = AF_INET;
  s.sin_addr.s_addr = inet_addr (ip_addr);
  s.sin_port = 1234;
  k = bind (sid, (struct sockaddr *) &s, sizeof (s));
  listen (sid, 1);
  l = sizeof (c);
  cid = accept (sid, (struct sockaddr *) &c, &l);
  pthread_create (&pt[0], NULL, read_th, (void *) 0);
  pthread_create (&pt[1], NULL, write_th, (void *) 0);
  pthread_join (pt[0], (void *) 0);
  pthread_join (pt[1], (void *) 0);
  close (sid);
  return 0;
}


void *
read_th (void *d)
{
  int r;
  char a[64];
  while (running)
    {
      r = read (cid, (void *) a, sizeof (a));
      if (r > 0)
	{
	  if (!strncmp (a, "bye", 3))
	    running = 0;
	  printf ("\nCLIENT: %s\n", a);
	}
    }
  close (cid);
  pthread_exit (0);
}


void *
write_th (void *d)
{
  int l;
  char a[64];

  while (running)
    {
      fprintf (stderr, "> ");
      fgets (a, sizeof (a), stdin);
      write (cid, (void *) a, strlen (a) + 1);
      if (!strncmp (a, "bye", 3))
	running = 0;
    }
  close (cid);
  pthread_exit (0);
}

