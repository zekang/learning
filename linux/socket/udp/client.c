#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	char *target = "127.0.0.1";
	short port = 8001;
	char buf[1024];
	int cid;
	ssize_t n;
	struct sockaddr_in addr;
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,target,&addr.sin_addr);
	addr.sin_port = htons(port);
	cid = socket(AF_INET,SOCK_DGRAM,0);
	while(fgets(buf,sizeof(buf),stdin)){
		n = sendto(cid,buf,strlen(buf),0,(struct sockaddr*) &addr,sizeof(struct sockaddr_in));
		printf("send %d\n",n);
		n = recvfrom(cid,buf,sizeof(buf),0,NULL,0);
		printf("recefrom %d\n",n);
		write(STDOUT_FILENO,buf,n);
	}
	close(cid);
}
