#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	char *target="127.0.0.1";	
	short port = 8000;
	int fd ,len;
	char buf[80];
	struct sockaddr_in addr;
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,target,&addr.sin_addr);
	addr.sin_port = htons(port);
	fd = socket(AF_INET,SOCK_STREAM,0);
	connect(fd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
	while(1){
			len =	read(STDIN_FILENO,buf,sizeof(buf));	
			write(fd,buf,len);
			len = read(fd,buf,sizeof(buf));
			write(STDOUT_FILENO,buf,len);
	}
	close(fd);
	return 0;	
}
