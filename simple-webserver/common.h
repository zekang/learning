#ifndef __COMMON_H__
#define __COMMON_H__
#include<WinSock2.h>
#define STATUS_NORMAL 1
#define STATUS_TYPES 2
#define STATUS_SERVERS 4
#define MAX_TYPES_ELEMENTS 128
#define MAX_SERVER_ELEMENTS 128

#ifdef __cplusplus 
extern "C" {
#endif
	typedef struct _config{
		int port;
		char default_type[32];
		struct {
			char *content_type;
			char *suffix;
		} types[MAX_TYPES_ELEMENTS];
		struct {
			char *server_name;
			char *root;
			char *index;
		} server[MAX_SERVER_ELEMENTS];
	}Config;

	typedef struct _requestHead{
		char path[256];
		char method[10];
		char host[32];
		int  port;

	} RequestHead;


	int parse_config(const char *path, Config *config /*out*/);

	int free_config(Config *config /*in*/);

	int parse_request(const char *head /*in*/, RequestHead *requestHead /*out*/);

	int free_request(RequestHead *requestHead /*in*/);

	int process_request(SOCKET socket, char *head, Config *server);
#ifdef __cplusplus
}
#endif
#endif