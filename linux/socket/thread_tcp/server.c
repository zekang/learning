#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>

#define PORT 8000
#define IP INADDR_ANY
#define IPLEN INET_ADDRSTRLEN
#define ADDRLEN sizeof(struct sockaddr_in)

typedef struct _client_info
{
		int fd;
		struct sockaddr_in client;
} client_info;
/**
 * ps -eLf ; view thread
 */

void *child(void *arg)/*{{{*/
{
		client_info *cli = (client_info *) arg;	
		char buf[1024];
		char ipbuf[INET_ADDRSTRLEN];
		int i,n;
		while(1){
				n = read(cli->fd,buf,sizeof(buf));
				if(n==0){
					printf("client closed\n");
					break;
				}
				printf("client:%s:%d\n",
								inet_ntop(AF_INET,&(cli->client.sin_addr),ipbuf,sizeof(ipbuf)),
								ntohs(cli->client.sin_port)
					  );
				for(i=0;i<n;i++){
					buf[i] = toupper(buf[i]);
				}
				write(cli->fd,buf,n);
		}
		close(cli->fd);
}/*}}}*/

int main(int argc,char *argv[])/*{{{*/
{
		int serv_fd,cli_fd;
		struct sockaddr_in serv_addr,cli_addr;
		socklen_t cli_len;
		pthread_attr_t attr;
		pthread_t tid;
		client_info cli_info;

		bzero(&serv_addr,ADDRLEN);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(IP);
		serv_addr.sin_port = htons(PORT);
		serv_fd	= socket(AF_INET,SOCK_STREAM,0);
		bind(serv_fd,(struct sockaddr *) & serv_addr,ADDRLEN);
		listen(serv_fd,128);
		pthread_attr_init(&attr);	
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		printf("accep connections......\n");
		while(1){
				cli_len = ADDRLEN;
				cli_fd = accept(serv_fd,(struct sockaddr *)&cli_addr,&cli_len);	
				if(cli_fd > 0){
						cli_info.fd = cli_fd;
						cli_info.client = cli_addr;
						pthread_create(&tid,&attr,child,(void*)&cli_info);
				}
		}
		return 0;
}/*}}}*/
