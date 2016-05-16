#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>


int main(int argc,char *argv[])
{
	int sid;
	struct sockaddr_in serv_addr,cli_addr;
	char buf[1024],ipstr[INET_ADDRSTRLEN];
	int i,n;
	socklen_t cli_addr_len;

	bzero(&serv_addr,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(8001);
	sid = socket(AF_INET,SOCK_DGRAM,0);	
	bind(sid,(struct sockaddr *) &serv_addr,sizeof(struct sockaddr_in));
	printf("Accepting ......\n");
	while(1){
		cli_addr_len = sizeof(struct sockaddr_in);
		n = recvfrom(sid,buf,sizeof(buf),0,	(struct sockaddr *)&cli_addr,&cli_addr_len);
		if(n == -1){
			perror("recvfrom");
			break;
		}
		printf("client:%s:%d\n",
			inet_ntop(AF_INET,&cli_addr.sin_addr,ipstr,sizeof(ipstr)),
			ntohs(cli_addr.sin_port));	
		for(i = 0;i<n;i++){
			buf[i] = toupper(buf[i]);
		}
		sendto(sid,buf,n,0,(struct sockaddr *)&cli_addr,sizeof(struct sockaddr_in));
	}
	close(sid);	
	return 0;
}
