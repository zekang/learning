#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include"common.h"

int server_init(const char *client_fifo_name,int *client_pfd/*out*/,const char *server_fifo_name,int *server_pfd) /*{{{*/
{
		int ret = 0;
		int fd = 0;  
		if(fifo_name == NULL || pfd==NULL){
				chat_error(CHAT_INFO,"fifo_name is null or pfd is null \n");
				return -1;
		}
		fifo_name = FIFO_SERVER;
		fd = chat_ensure_fifo(fifo_name,O_RDONLY|O_NONBLOCK);
		if(fd < 0){
				chat_error(CHAT_INFO,"open fifo file %s failed\n",fifo_name);	
				ret = -1;
				goto END;
		}
		tmp = (USER **) calloc(sizeof(MSG *),count);
		if(tmp == NULL){
				ret = -1;
				perror("calloc");
				goto END;
		}

END:
		if(ret == -1){
				if(tmp != NULL){
						free(tmp);
				}
				if(fd>0){
						close(fd);
				}
		}else{
				*users = tmp;
				*pfd = fd;
		}
		return ret;
}/*}}}*/

int server_destroy(USER **users /*in*/,int count,const char *fifo_name,int fd)/*{{{*/
{
		int i = 0;
		if(users && count>0){
				for(i;i<count;i++){
						if(users[i] != NULL){
								free(users[i]);	
								chat_error(CHAT_DEBUG,"free users[%d]\n",i);
						}
				}
				free(users);
		}
		if(fd>-1){
				close(fd);
				chat_error(CHAT_DEBUG,"close file descrtor %d\n",fd);
		}
		if(fifo_name == NULL){
				fifo_name = FIFO_SERVER;
		}
		unlink(fifo_name);
		chat_error(CHAT_DEBUG,"unlink file %s\n",fifo_name);
}/*}}}*/

int server_read(MSG *msg,int fd) /*{{{*/
{
		ssize_t readlen ;
		int ret = 0;
		do{
				readlen = read(fd,msg,sizeof(MSG));
				if(readlen == -1){
						if(errno != EAGAIN){
								perror("read");
								ret = -1;
								break;
						}
				}else{
						if(readlen == sizeof(MSG)){
								ret = 0;
								break;
						}
				}
				sleep(1);
		}while(1);
		return ret;
}/*}}}*/

int server_login(USER **users,int index,const char *username) /*{{{*/
{
		USER *user = (USER *) calloc(sizeof(USER),1);	
		if(user == NULL){
				perror("calloc");
				return -1;
		}
		strncpy(user->name ,username,5);
		user->status = 1;
		user->fd = chat_ensure_fifo(user->name,O_WRONLY); 
		if(user->fd<0){
				free(user);
				return -1;
		}
		users[index] = user; 
		return 0;
}/*}}}*/

int server_user_lookup(USER **users,int count,const char *username)/*{{{*/
{
		int i = 0;
		USER *user;
		for(i;i<count;i++){
				user = users[i];
				if(strcmp(user->name,username) == 0){
						return i;
				}
		}
		return -1;
}/*}}}*/

int server_logout(USER **users,int index ) /*{{{*/
{
		USER *user = users[index];
		if(user == NULL){
				return -1;
		}
		if(user->fd > -1){
				close(user->fd);
		}
		unlink(user->name);
		free(user);
		users[index] = NULL; 
		return 0;
}/*}}}*/

int server_write_client(int fd,MSG *msg)/*{{{*/
{
		return write(fd,msg,sizeof(MSG)) ==  -1 ? -1:0;	
}/*}}}*/

int server_run(USER **users,int count,int fd) /*{{{*/
{
		int ret = 0;
		MSG msg = {0};
		int num = 0;
		int index;
		do{
				if(server_read(&msg,fd) == -1){
						ret = -1;
						break;
				}
				if(IS_LOGIN(&msg)) {
						index = server_user_lookup(users,count,msg.src); 
						chat_error(CHAT_DEBUG,"login message received \n");
						if(index>-1){
								chat_error(CHAT_DEBUG,"user %s has login \n",msg.src);
								continue;
						}
						if(num >= count){
								chat_error(CHAT_DEBUG,"login user %s failed ,reached max count \n",msg.src);
								continue;
						}
						if( server_login(users,num,msg.src) == -1){
								chat_error(CHAT_DEBUG,"login user %s failed \n",msg.src);
						}else{
								msg.action = MSG_CONTENT;	
								strcpy(msg.data,"login success\n");
								strcpy(msg.dest,msg.src);
								strcpy(msg.src,"sys");
								write((users[num])->fd,&msg);
								num++;
						}
				}else if(IS_LOGOUT(&msg)){
						index = server_user_lookup(users,count,msg.src); 
						chat_error(CHAT_DEBUG,"logout message received \n");
						if(index < 0){
								chat_error(CHAT_DEBUG,"user %s has loginout \n",msg.src);
						}else{
								server_logout(users,index);		
						}
				}else if(IS_CONTENT(&msg)){
						index = server_user_lookup(users,count,msg.dest); 
						chat_error(CHAT_DEBUG,"content message received \n");
						if(index>-1 && users[index] != NULL){		
								write((users[index])->fd,&msg);
						}else{
							chat_error(CHAT_DEBUG,"user %s not found\n",msg.dest);
						}
				}else {
						chat_error(CHAT_DEBUG,"unknow message received \n");
				}
		}while(1);
		return ret;
}/*}}}*/

int main(int argc,char *argv[]) /*{{{*/
{
		int ret;
		int fd;
		if (argc < 2){
			chat_error(CHAT_FATAL,"usage:client username[1-5]\n");
		}
		ret = client_init(argv[1],&fd);
		if(ret != 0){
				goto END;
		}
		client_run(fd);	
END:
		client_destroy(argv[1]);
		return 0;
}/*}}}*/

