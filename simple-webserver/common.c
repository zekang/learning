#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include<WinSock2.h>
#include<io.h>
#include "common.h"


#define strdup _strdup
#define access _access
void debug(char *format, ...)
{
	return;
	va_list ap;
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
}

static int parse_line(char *line/*in*/, char *key/*out*/, char *value/*out*/,char delim)
{
	char *pos, *start,*end;
	start = line;
	if (line == NULL || key == NULL || value == NULL){
		return -1;
	}
	pos = strchr(start, delim);
	if (pos == NULL){
		debug("in %s = not found\n" ,line);
		return -1;
	}
	end = pos - 1;
	while (start < end && isspace(*start)){
		start++;
	}
	while (end > start && isspace(*(end-1))){
		end--;
	}
	if (start > end){
		debug("in %s key is empty \n",line );
		return -1;
	}
	if (!isspace(*end)){
		end++;
	}
	memcpy(key, start, end - start);
	key[end - start] = 0;

	pos++;
	start = pos;
	end = start + strlen(start);

	while (start < end && isspace(*start)){
		start++;
	}
	while (end > start && isspace(*(end-1))){
		end--;
	}
	if (start < end){
		memcpy(value, start, end - start);
		value[end - start] = 0;
	}
	else{
		value[0] = 0;
	}
	return 0;
}

int parse_config(const char *path/*in*/, Config *config /*out*/)
{
	FILE *fp = NULL;
	int ret = -1;
	char buf[4096];
	char key[128], value[128];

	int status,type_index=0,server_index=0;
	fp = fopen(path, "rb");
	if (fp == NULL){
		perror("fopen");
		goto END;
	}
	status = STATUS_NORMAL;
	while (fgets(buf, sizeof(buf), fp)){
		if (strchr(buf, '}')){
			if (status == STATUS_SERVERS){
				server_index++;
			}
			status = STATUS_NORMAL;
			continue;
		}
		if (strchr(buf, '{')){
			if (strstr(buf, "types")){
				status = STATUS_TYPES;
			}
			else if (strstr(buf, "server")){
				status = STATUS_SERVERS;
			}
			continue;
		}
		if (parse_line(buf, key, value,'=') == 0){
			if (status == STATUS_NORMAL){
				if (strcmp(key, "listen") == 0){
					config->port = atoi(value);
				}
				else if (strcmp(key, "default_type") == 0){
					strcpy(config->default_type, value);
				}
				else{
					printf("error config");
					ret = -1;
					break;
				}
			}
			else if (status == STATUS_TYPES){
				if (type_index == MAX_TYPES_ELEMENTS){
					printf("too much types config");
					ret = -1;
					break;
				}
				config->types[type_index].content_type = strdup(key);
				config->types[type_index].suffix = strdup(value);
				type_index++;
			}
			else if (status == STATUS_SERVERS){
				if (server_index == MAX_SERVER_ELEMENTS){
					printf("too much server config");
					ret = -1;
					break;
				}
				if (strcmp(key, "server_name") == 0){
					config->server[server_index].server_name = strdup(value);
				}
				else if (strcmp(key, "root") == 0){
					config->server[server_index].root = strdup(value);
				}
				else if (strcmp(key, "index") == 0){
					config->server[server_index].index = strdup(value);
				}
			}
		}
	}
	fclose(fp);
	ret = 0;
END:
	return ret;
}

int free_config(Config *config /*in*/)
{
	if (config == NULL){
		return 0;
	}
	int i;
	for (i = 0; i < MAX_TYPES_ELEMENTS; i++){
		if (config->types[i].content_type != NULL){
			free(config->types[i].content_type);
			config->types[i].content_type = NULL;
		}
		if (config->types[i].suffix != NULL){
			free(config->types[i].suffix);
			config->types[i].suffix = NULL;
		}
	}
	for (i = 0; i < MAX_SERVER_ELEMENTS; i++){
		if (config->server[i].server_name != NULL){
			free(config->server[i].server_name);
			config->server[i].server_name = NULL;
		}
		if (config->server[i].root != NULL){
			free(config->server[i].root);
			config->server[i].root = NULL;
		}
		if (config->server[i].index != NULL){
			free(config->server[i].index);
			config->server[i].index = NULL;
		}
	}
	return 0;
}

int parse_request(const char *head /*in*/, RequestHead *requestHead /*out*/)
{
	
	return 0;
}

int free_request(RequestHead *requestHead /*in*/)
{
	return 0;
}

int process_request(SOCKET socket, char *head,Config *server)
{
	char buf[1024], key[1024], value[1024],headBuf[1024];
	char *delim = "\r\n",*token;
	char *context;
	int len, i;
	char *root = NULL, *content_type = NULL, *pos, *index = NULL;
	RequestHead requestHead = { 0 };
	
	do{
		token = strtok_s(head, delim, &context);
		head = NULL;
		if (token){
			if (parse_line(token, key, value, ' ') == 0){
				if (strcmp(key, "GET") == 0){
					strcpy(requestHead.method, key);
					if (parse_line(value, key, buf, ' ') == 0){
						strcpy(requestHead.path, key);
					}
				}
				else if (strcmp(key, "Host:") == 0){
					if (parse_line(value, key, buf, ':') == 0){
						strcpy(requestHead.host, key);
						requestHead.port = atoi(buf);
					}
				}
			}
		}
	} while (token);

	printf("%s:%d\n", requestHead.host, requestHead.port);
	printf("%s:%s\n", requestHead.method, requestHead.path);
	if (requestHead.port != server->port){
		return 0;
	}

	for (i = 0; i < MAX_SERVER_ELEMENTS; i++){
		if (server->server[i].server_name == NULL){
			continue;
		}
		if (strcmp(requestHead.host, server->server[i].server_name) == 0){
			root = server->server[i].root;
			index = server->server[i].index;
			break;
		}
	}

	//查找服务器文件 buf保存文件路径
	pos = strchr(requestHead.path, '?');
	if (pos){
		strcpy(buf, root);
		if (pos - requestHead.path == 1){
			strcat(buf, "/");
			strcat(buf, index);
		}
		else{
			strncat(buf, requestHead.path, pos - requestHead.path);
		}
	}
	else{
		if (strlen(requestHead.path) == 1){
			sprintf(buf, "%s/%s", root, index);
		}
		else{
			if (*(requestHead.path + strlen(requestHead.path)-1) == '/'){
				sprintf(buf, "%s%s%s", root, requestHead.path,index);
			}
			else{
				sprintf(buf, "%s%s", root, requestHead.path);
			}
		}
	}
	if (access(buf, 0) != 0){
		len = sprintf(headBuf, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nStatus: 404 Not Found\r\n\r\n<h1>404 Not Found</h1>");
		send(socket, headBuf, len, 0);
		return 0 ;
	}


	//根据后缀计算content-type
	pos = strrchr(buf, '.');
	if (pos != NULL && strlen(pos) > 1) {
		pos++;
		strcpy(key, pos);
		for (i = 0; i < MAX_TYPES_ELEMENTS; i++){
			if (server->types[i].suffix == NULL){
				continue;
			}
			if ( (pos = strstr(server->types[i].suffix, key))!=NULL){
				content_type = server->types[i].content_type;
				break;
			}
		}
	}
	if (content_type == NULL){
		content_type = server->default_type;
	}

	len = sprintf(headBuf, "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=utf-8\r\nStatus: 200 OK\r\n\r\n", content_type);
	send(socket, headBuf, len, 0);
	FILE *fp = fopen(buf, "rb");
	if (fp){
		do{
		len = fread(buf, sizeof(char), sizeof(buf), fp);
		if (len>0){
			send(socket, buf, len, 0);
		}
		} while (!feof(fp));
		fclose(fp);
	}
	return 0;
}