#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h>

void *snd(void *arg)
{
	char b[100];
	int cid=*((int*)arg);
	struct sockaddr_in c;
	c.sin_port=htons(1234);
	c.sin_family=AF_INET;
	inet_aton("225.0.0.0",&c.sin_addr);
	while(1)
	{
		fgets(b,sizeof(b),stdin);
		sendto(cid,b,sizeof(b),0,(struct sockaddr*)&c,sizeof(c));
	}
}
void *rcv(void *arg)
{
	char a[100];
	int v=-3;
	struct ip_mreq m;
	int cid=*((int*)arg);
	struct sockaddr_in c;
	c.sin_family=AF_INET;
	c.sin_port=htons(1234);
	inet_aton("0.0.0.0",&c.sin_addr);
	setsockopt(cid,SOL_SOCKET,SO_REUSEADDR,(void*)&v,sizeof(v));
	bind(cid,(struct sockaddr*)&c,sizeof(c));
	inet_aton("0.0.0.0",&m.imr_interface);
	inet_aton("225.0.0.0",&m.imr_multiaddr);
	setsockopt(cid,IPPROTO_IP,IP_ADD_MEMBERSHIP,(void*)&m,sizeof(m));
	//inet_aton("225.0.0.0",&s.sin_addr);
	while(1)
	{
		int l=sizeof(c);
		recvfrom(cid,a,sizeof(a),0,(struct sockaddr*)&c,&l);
		printf("Message received : ");
		puts(a);
	}
}
		
int main()
{
	int sid=socket(AF_INET,SOCK_DGRAM,0);
	pthread_t s,c1,c2,c3;
	pthread_create(&s,NULL,snd,(void*)&sid);
	pthread_create(&c1,NULL,rcv,(void*)&sid);
	pthread_create(&c2,NULL,rcv,(void*)&sid);
	pthread_create(&c3,NULL,rcv,(void*)&sid);
	pthread_join(s,NULL);
	pthread_join(c1,NULL);
	pthread_join(c2,NULL);
	pthread_join(c3,NULL);
	/*struct sockaddr_in s;
	s.sin_family=AF_INET;
	s.sin_port=htons(1234);
	inet_aton("225.0.0.0",&s.sin_addr);*/
	return 0;
}
	
	
