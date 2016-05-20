#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<poll.h>
#include<string.h>

#define PORT 8000;
#define OPEN_MAX 1024;
int main(int argc,char *argv[])
{
	int serv_fd,cli_fd,max_index,read_len,i,j,nready;
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t cli_addr_len;
	char ipbuf[INET_ADDRSTRLEN],buf[1024];
	struct pollfd clients[OPEN_MAX];	
	nfds_t max;
	for(i=0;i<OPEN_MAX;i++){
		clients[i].fd = -1;
	}
	max = 0;
	
	return 0;	
}
