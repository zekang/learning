#include<stdio.h>
#include<stdlib.h>
#include<sys/select.h>
#include<time.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

#define MAX_CLIENT 1024
#define PORT 8000
int main(int argc,char *argv[])
{
		int clients[MAX_CLIENT];	
		struct sockaddr_in serv_addr,cli_addr;
		socklen_t cli_addr_len;
		char ipbuf[INET_ADDRSTRLEN];
		char buf[1024];
		int read_len,i,j,max_index,max_fd,nready;
		int serv_fd,cli_fd;
		fd_set allset,readset;

		serv_fd = socket(AF_INET,SOCK_STREAM,0);
		bzero(&serv_addr,sizeof(serv_addr));	
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(PORT);
		bind(serv_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
		listen(serv_fd,20);

		for(i = 0;i<MAX_CLIENT;i++){
				clients[i] = -1;
		}
		max_fd = serv_fd;
		max_index = -1;

		FD_ZERO(&allset);
		FD_SET(serv_fd,&allset);
		printf("Accept connection...\n");
		while(1){
				readset = allset;
				nready = select(max_fd+1,&readset,NULL,NULL,NULL);	
				if(nready<0){
						perror("select");
						break;
				}
				if(FD_ISSET(serv_fd,&readset)){
						nready--;
						cli_addr_len = sizeof(struct sockaddr_in);
						cli_fd = accept(serv_fd,(struct sockaddr *)&cli_addr,&cli_addr_len);
						for(i=0;i<MAX_CLIENT;i++){
							if(clients[i] < 0){
								clients[i] = cli_fd;
								break;
							}
						}
						if(i>=MAX_CLIENT){
							printf("too many clients \n");
							break;
						}
						printf("client %s:%d\n",
							inet_ntop(AF_INET,&cli_addr.sin_addr,ipbuf,sizeof(ipbuf)),
							ntohs(cli_addr.sin_port)
							);
						FD_SET(cli_fd,&allset);
						max_index = i > max_index ? i:max_index;
						max_fd = cli_fd > max_fd ? cli_fd : max_fd;
						//printf("max_index:%d,max_fd:%d,nready:%d\n",max_index,max_fd,nready);
				}
				for(i=0;nready>0 && i <= max_index;i++){
					if(clients[i] < 0) continue;
					if(FD_ISSET(clients[i],&readset)){
						//printf("i:%d\n",i);
						read_len = read(clients[i],buf,sizeof(buf));								
						if(read_len == 0){
							FD_CLR(clients[i],&allset);
							close(clients[i]);
							clients[i] = -1;
							continue;
						}
						for(j=0;j<read_len;j++){
							buf[j] = toupper(buf[j]);
						}
						write(clients[i],buf,read_len);
						nready--;
					}
				}
		}
		close(serv_fd);
		return 0;
}
