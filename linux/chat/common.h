#ifndef __CHAT_COMMON_H__
#define __CHAT_COMMON_H__
#define DEBUG_MODE
#define FIFO_SERVER "fifo_server"
#define MAX_USER  20
#ifdef __cplusplus
extern "C" {
#endif

struct _MSG
{
	char src[6];
	char dest[6];
	char data[114];
	short action;
};
typedef struct _MSG MSG;

struct _user
{
	char name[6];
	short status;
	int  fd;
};
typedef struct _user USER;

#define MSG_LOGIN   1 << 0
#define MSG_LOGOUT  1 << 1
#define MSG_CONTENT 1 << 2

#define IS_LOGIN(msg)  	((msg)->action & MSG_LOGIN)
#define IS_LOGOUT(msg)  ((msg)->action & MSG_LOGOUT)
#define IS_CONTENT(msg) ((msg)->action & MSG_CONTENT)
enum  _chat_error_type{
		CHAT_DEBUG,
		CHAT_INFO, 
		CHAT_FATAL 
};
typedef enum _chat_error_type C_ERTYPE;

void chat_error(C_ERTYPE type,char *format,...);
int  chat_ensure_fifo(const char *filename,int mode);
#ifdef __cplusplus
}
#endif
#endif
