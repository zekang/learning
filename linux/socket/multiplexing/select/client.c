#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>


#define IP "127.0.0.1"
#define PORT 8000

int main(int argc,char *argv[])
{
		int cli_fd;
		struct sockaddr_in addr;
		char buf[1024];
		int len;

		bzero(&addr,sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		inet_pton(AF_INET,IP,&addr.sin_addr);
		addr.sin_port = htons(PORT);
		cli_fd = socket(AF_INET,SOCK_STREAM,0);		
		connect(cli_fd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
		while(1){
				len = read(STDIN_FILENO,buf,sizeof(buf));
				if(strncmp(buf,"quit",4)==0){
						break;
				}
				write(cli_fd,buf,len);
				len = read(cli_fd,buf,sizeof(buf));
				buf[len] = '\0';
				puts(buf);
		}
		return 0;
}
