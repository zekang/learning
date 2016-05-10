#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdarg.h>
#include "common.h"

void chat_error( C_ERTYPE type,char *format,...) /*{{{*/
{
		va_list ap;
		if(type == CHAT_DEBUG){
			#ifndef DEBUG_MODE
			return;
			#endif
		}
		if(format != NULL){
				va_start(ap,format);
				vfprintf(stderr,format,ap);
				va_end(ap);
		}
		if(type == CHAT_FATAL){
				exit(-1);
		}
}/*}}}*/

/**
 * @param pipe name
 * @return file descriptor on SUCCESS ,-1 on FAILURE
 */
int chat_ensure_fifo(const char *filename,int mode) /*{{{*/
{
		struct stat st;
		int fd;
		if(filename == NULL){
				return -1;
		}
		//file_exists
		if(access(filename,F_OK) == 0){
			if(stat(filename,&st) == 0){
				if(!S_ISFIFO(st.st_mode)){
					chat_error(CHAT_INFO,"file %s is exists,buf is not named pipe,please check\n",filename);		
					return -1;
				}
			}else{
				perror("stat");
				return -1;
			}
		}else{
			umask(0);
			if(mkfifo(filename,0644) == -1){
				perror("mkfifo");
				return -1;
			}
		}
		fd = open(filename,mode);
		if(fd < 0){
			perror("open");
			return -1;
		}
		return fd;
}/*}}}*/
