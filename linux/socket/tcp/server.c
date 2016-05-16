#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc,char *argv[])/*{{{*/
{
	int sfd,cfd;		
	struct sockaddr_in serv_addr,cli_addr;
	int address_len;
	char ipstr[INET_ADDRSTRLEN];
	char buf[80];
	int i,n;
	bzero(&serv_addr,sizeof(struct sockaddr_in));
	sfd = socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(8000);
	bind(sfd,(struct sockaddr *) &serv_addr,sizeof(struct sockaddr_in));
	listen(sfd,20);
	printf("Accepting...\n");
	while(1){
			address_len = sizeof(struct sockaddr_in);
			cfd = accept(sfd,(struct sockaddr *) &cli_addr,&address_len);
			printf("client %s:%d\n",
			inet_ntop(AF_INET,&cli_addr.sin_addr,ipstr,sizeof(ipstr)),
			ntohs(cli_addr.sin_port));
			n = read(cfd,buf,sizeof(buf));
			if(n == -1){
				perror("read");
				break;
			}
			for(i=0;i<n;i++){
				buf[i] = toupper(buf[i]);
			}
			write(cfd,buf,n);
			close(cfd);
	}
	close(sfd);
	return 0;
}/*}}}*/
