#include<stdio.h>
#include<stdlib.h>
#include<sys\types.h>
#define  FD_SETSIZE 1024
#include<WinSock2.h>
#include "common.h"
#ifdef MAXIMUM_WAIT_OBJECTS
#undef MAXIMUM_WAIT_OBJECTS
#endif
#define MAXIMUM_WAIT_OBJECTS 1024
#pragma comment(lib, "ws2_32.lib")


int run(Config *config)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		return -1;
	}
	SOCKET socketCli;
	SOCKET clients[MAXIMUM_WAIT_OBJECTS];
	int i,nready,readlen;
	fd_set readset,allset;
	int addr_len;
	SOCKET socketServ = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr, cli_addr;
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(config->port);

	bind(socketServ, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(socketServ, 128);
	printf("listen at : %d \n", config->port);
	FD_ZERO(&allset);
	FD_SET(socketServ, &allset);
	for (i = 0; i < MAXIMUM_WAIT_OBJECTS; i++){
		clients[i] = 0;
	}
	clients[0] = socketServ;
	char buf[8192];
	while (1){
		addr_len = sizeof(struct sockaddr_in);
		readset = allset;
		nready = select(1024, &readset, NULL, NULL, NULL);
		if (nready < 0){
			perror("select");
			break;
		}
		if (FD_ISSET(socketServ, &readset)){
			socketCli = accept(socketServ, (struct sockaddr*)&cli_addr, &addr_len);
			printf("client %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
			for (i = 1; i < MAXIMUM_WAIT_OBJECTS; i++){
				if (clients[i] == 0){
					clients[i] = socketCli;
					break;
				}
			}
			nready--;
			FD_SET(socketCli, &allset);
			if (i == MAXIMUM_WAIT_OBJECTS){
				printf("Reach Max\n");
				break;
			}
		}
		for (i = 1; i < MAXIMUM_WAIT_OBJECTS && nready>0; i++){
			if (clients[i] == 0) continue;
			if (FD_ISSET(clients[i], &readset)){
				nready--;
				if ((readlen = recv(clients[i], buf, sizeof(buf), 0)) == 0){
					closesocket(clients[i]);
					FD_CLR(clients[i], &allset);
					clients[i] = 0;
					printf("client close\n");
					continue;
				}
				if (readlen < 0){
					continue;
				}
				buf[readlen] = 0;
				process_request(clients[i], buf, config);
				shutdown(clients[i], SD_SEND);
			}
		}
	}
	closesocket(socketServ);
	WSACleanup();
	return 0;
}

int main(int argc, char *argv[])
{
	int ret;
	char *path = "D:/whq/learning/socket/Release/http.conf";

	Config server = { 0 };
	if (argc > 1){
		path = argv[1];
	}
	if ((ret = parse_config(path, &server)) != 0){
		fprintf(stderr, "parse config file %s failed\n", path);
		goto END;
	}
	run(&server);
END:
	if (ret == 0){
		free_config(&server);
	}
	system("pause");
	return 0;
}