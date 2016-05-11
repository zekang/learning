#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include"common.h"

int client_write(int fd,MSG *msg)/*{{{*/
{
		return write(fd,msg,sizeof(MSG)) ==  -1 ? -1:0;	
}/*}}}*/

int client_init(const char *client_fifo_name,int *client_pfd/*out*/,const char *server_fifo_name,int *server_pfd) /*{{{*/
{
		int ret = 0;
		int sfd = -1,cfd = -1; 
		MSG msg={0};
		if(client_fifo_name == NULL || client_pfd==NULL || server_pfd == NULL){
				chat_error(CHAT_INFO,"client_fifo_name is null or client_pfd is null or server_pfd is null\n");
				return -1;
		}
		if(server_fifo_name == NULL){
				server_fifo_name = FIFO_SERVER;
		}
		sfd = chat_ensure_fifo(server_fifo_name,O_WRONLY);
		if(sfd < 0){
				chat_error(CHAT_INFO,"open server fifo file %s failed\n",server_fifo_name);	
				ret = -1;
				goto END;
		}

		cfd = chat_ensure_fifo(client_fifo_name,O_RDONLY|O_NONBLOCK);
		if(cfd < 0){
				chat_error(CHAT_INFO,"open client fifo file %s failed\n",client_fifo_name);	
				ret = -1;
				goto END;
		}

		//login
		msg.action = MSG_LOGIN;
		strcpy(msg.src,client_fifo_name);
		strcpy(msg.dest,"sys");
		client_write(sfd,&msg);
END:
		if(ret == -1){
				if(sfd>=0){
						close(sfd);
				}
				if(cfd>=0){
						close(cfd);
				}
		}else{
				*client_pfd = cfd;
				*server_pfd = sfd;
		}
		return ret;
}/*}}}*/

int client_run(const char *username,int sfd,int cfd) /*{{{*/
{
		int flags;
		char buf[1024];
		int ret = 0,len =0;
		MSG msg = {0};
		char *data;
		flags = fcntl(STDIN_FILENO,F_GETFL);	
		if( flags  == -1){
				chat_error(CHAT_DEBUG,"fcntl %s\n",strerror(errno));
				return -1;
		}
		flags |=  O_NONBLOCK;
		if (fcntl(STDIN_FILENO,F_SETFL,flags)== -1){
				chat_error(CHAT_DEBUG,"fcntl set %s\n",strerror(errno));
				return -1;
		}
		do{
				len = read(STDIN_FILENO,buf,sizeof(buf));
				if(len > 0){
						buf[len - 1] = '\0';
						if(strcasecmp(buf,"quit") == 0){
							break;
						}
						data = strchr(buf,':');	
						if(data == NULL){
							printf("msg format error,eg:username:msg content\n");
						}else{
							*data='\0';
							msg.action=MSG_CONTENT;
							strcpy(msg.data,data+1);
							strcpy(msg.src,username);
							strcpy(msg.dest,buf);
							client_write(sfd,&msg);
						}
				} else {
						if(errno != EAGAIN){
								chat_error(CHAT_DEBUG,"open :%s\n",strerror(errno));
								ret = -1;
								break;
						}
				}

				//check have message
				len = read(cfd,&msg,sizeof(MSG));
				if(len >0){
						if(len != sizeof(MSG)){
								chat_error(CHAT_DEBUG,"read len error %d\n",len);	
								ret = -1;
								break;
						}else{
								printf("%s:%s\n",msg.src,msg.data);
						}
				}else{
						if(errno != EAGAIN){
								chat_error(CHAT_DEBUG,"open :%s\n",strerror(errno));
								ret = -1;
								break;
						}
				}
				sleep(1);
		}while(1);
		return ret;
}/*}}}*/

int client_shutdown(const char *username,int sfd,int cfd)/*{{{*/
{
		MSG msg={0};	

		if(sfd>-1){
				//logout
				msg.action = MSG_LOGOUT;
				strcpy(msg.src,username);
				strcpy(msg.dest,"sys");
				client_write(sfd,&msg);
				close(sfd);
		}
		if(cfd>-1){
				close(cfd);
		}
		return 0;
}/*}}}*/

int main(int argc,char *argv[]) /*{{{*/
{
		int ret;
		int cfd,sfd;
		if (argc < 2){
				chat_error(CHAT_FATAL,"usage:client username[1-5]\n");
		}
		if(strlen(argv[1])>5){
				chat_error(CHAT_FATAL,"username[1-5]\n");
		}
		ret = client_init(argv[1],&cfd,NULL,&sfd);
		if(ret != 0){
				goto END;
		}
		client_run(argv[1],sfd,cfd);	
END:
		client_shutdown(argv[1],sfd,cfd);
		return 0;
}/*}}}*/

