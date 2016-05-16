#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<string.h>

#define PORT 8000
#define IP   INADDR_ANY

void close_children(int signo)
{
	  waitpid(0,NULL,WNOHANG);
}
int main(int argc,char *argv[])
{
		int serv_fd,cli_fd;
		struct sockaddr_in serv_addr,cli_addr;
		socklen_t socklen;
		pid_t pid;
		struct sigaction act;
		char ipstr[INET_ADDRSTRLEN];
		char buf[1024];
		int i,n;
		bzero(&act,sizeof(struct sigaction));
		sigemptyset(&act.sa_mask);
		act.sa_handler = close_children;
		act.sa_flags = 0;
		sigaction(SIGCHLD,&act,NULL);
		bzero(&serv_addr,sizeof(struct sockaddr_in));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(IP);
		serv_addr.sin_port = htons(PORT);
		serv_fd = socket(AF_INET,SOCK_STREAM,0);
		bind(serv_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in));
		listen(serv_fd,128);
		printf("Accept connection.....\n");
		while(1){
				socklen = sizeof(struct sockaddr_in);	
				cli_fd = accept(serv_fd,(struct sockaddr*)&cli_addr,&socklen);
				if(cli_fd == -1){
						continue;
				}
				pid = fork();	
				if(pid == 0){
						close(serv_fd);	
						while(1){
								n = read(cli_fd,buf,sizeof(buf));
								if(n==0){
										printf("client closed\n");
										break;
								}
								printf("client %s:%d \n",
												inet_ntop(AF_INET,&cli_addr.sin_addr,ipstr,sizeof(ipstr)) ,
												ntohs(cli_addr.sin_port)
									  );	
								for(i=0;i<n;i++){
										buf[i] = toupper(buf[i]);	
								}
								write(cli_fd,buf,n);
						}
						close(cli_fd);
						return 0;
				}else if(pid >0){
						close(cli_fd);
				}
		}
		close(serv_fd);
		return 0;
}
