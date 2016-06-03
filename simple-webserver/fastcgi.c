#include<stdio.h>
#include<stdlib.h>
#include"fastcgi.h"
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int buildPacket(char type, char *content /*in*/,int content_len, unsigned short requestId, char *retval /*out*/,int *retval_len/*out*/)
{
	fcgi_header head;
	int offset = 0;
	int retval_offset = 0;
	int packet_content_len = 0;
	int is_end;
	*retval_len = 0;
	do{
		if (content_len - offset < 65535 - FCGI_HEADER_LEN){
			packet_content_len = content_len - offset;
			is_end = 1;
		}
		else{
			packet_content_len = 65535 - FCGI_HEADER_LEN;
			is_end = 0;
		}

		head.version = FCGI_VERSION_1;
		head.type = type;
		head.requestIdB1 = requestId >> 8 & 0XFF;
		head.requestIdB0 = requestId & 0XFF;
		head.contentLengthB1 = packet_content_len >> 8 & 0xFF;
		head.contentLengthB0 = packet_content_len & 0XFF;
		head.paddingLength = 0;
		head.reserved = 0;
		memcpy(retval + retval_offset, &head, FCGI_HEADER_LEN);
		retval_offset += FCGI_HEADER_LEN;
		if (packet_content_len > 0){
			memcpy(retval + retval_offset, content + offset, packet_content_len);
			retval_offset += packet_content_len;
			offset += packet_content_len;
		}
		*retval_len += FCGI_HEADER_LEN + packet_content_len;
		if (is_end){
			break;
		}
	} while (1);
	return 0;
}

int buildNvpair(char *name /*in*/, char *value /*in*/, char *retval /*out*/,int *retval_len /*out*/)
{
	int name_len = strlen(name);
	int value_len = strlen(value);
	int index = 0;
	
	if (name_len < 128){
		retval[index++] = name_len;
	}
	else{
		retval[index++] = name_len >> 24 | 0X80;
		retval[index++] = name_len >> 16 | 0XFF;
		retval[index++] = name_len >> 8  | 0XFF;
		retval[index++] = name_len | 0XFF;
	}

	if (value_len < 128){
		retval[index++] = value_len;
	}
	else{
		retval[index++] = value_len >> 24 | 0X80;
		retval[index++] = value_len >> 16 | 0XFF;
		retval[index++] = value_len >> 8 | 0XFF;
		retval[index++] = value_len | 0XFF;
	}
	memcpy(retval + index, name, name_len);
	index += name_len;
	memcpy(retval + index, value, value_len);
	index += value_len;
	*retval_len = index;
	return 0;
}


int main(int argc, char *argv[])
{
	WSADATA wsaData;
	struct sockaddr_in serv_addr;
	SOCKET socketCli;
	char sendBuf[204800], buf[102400] = { 0 };
	char content[] = { 0, FCGI_RESPONDER, 0, 0, 0, 0, 0, 0 };
	char paramBuf[1024];
	int buf_len, pos=0,param_pos=0;
	fcgi_header head = { 0 };
	int response_len = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)){
		return -1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(9000);
	serv_addr.sin_family = AF_INET;
	socketCli = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(socketCli, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == INVALID_SOCKET){
		printf("connect error\n");
		system("pause");
		return -1;
	}
 
	buildPacket(FCGI_BEGIN_REQUEST, content , sizeof(content), 1, buf, &buf_len);
	memcpy(sendBuf + pos, buf, buf_len);
	pos += buf_len;
	
	buildNvpair("REQUEST_METHOD", "POST", buf, &buf_len);
	memcpy(paramBuf + param_pos, buf, buf_len);
	param_pos += buf_len;

	buildNvpair("SCRIPT_FILENAME", "d:/www/a.php", buf, &buf_len);
	memcpy(paramBuf + param_pos, buf, buf_len);
	param_pos += buf_len;

	buildNvpair("QUERY_STRING", "a=1&b=2", buf, &buf_len);
	memcpy(paramBuf + param_pos, buf, buf_len);
	param_pos += buf_len;

	buildNvpair("CONTENT_TYPE", "application/x-www-form-urlencoded", buf, &buf_len);
	memcpy(paramBuf + param_pos, buf, buf_len);
	param_pos += buf_len;

	buildNvpair("CONTENT_LENGTH", "7", buf, &buf_len);
	memcpy(paramBuf + param_pos, buf, buf_len);
	param_pos += buf_len;
	
	buildNvpair("PATH_INFO", "/", buf, &buf_len);
	memcpy(paramBuf + param_pos, buf, buf_len);
	param_pos += buf_len;


	buildPacket(FCGI_PARAMS, paramBuf, param_pos, 1, buf, &buf_len);
	memcpy(sendBuf + pos, buf, buf_len);
	pos += buf_len;

	
	buildPacket(FCGI_PARAMS, "", 0, 1, buf, &buf_len);
	memcpy(sendBuf + pos, buf, buf_len);
	pos += buf_len;
	
	buildPacket(FCGI_STDIN, "c=3&d=4", 7, 1, buf, &buf_len);
	memcpy(sendBuf + pos, buf, buf_len);
	pos += buf_len;

	send(socketCli, sendBuf, pos, 0);
	do{
		if (recv(socketCli, (char *)&head, sizeof(head), 0) == 0){
			break;
		}

		response_len = ((head.contentLengthB1) << 8) + head.contentLengthB0;
		recv(socketCli, buf, response_len, 0);
		buf[response_len] = 0;
		printf("%s",buf);
		if (head.type == FCGI_END_REQUEST){
			break;
		}
	} while (1);
	system("pause");
	return 0;
}
