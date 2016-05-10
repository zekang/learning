#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include "lib.h"
char * trim(char *input,int len) /*{{{*/
{
		if(input == NULL){
				return input;
		}
		char *begin,*end;
		begin = input;
		end = input + len;

		while(begin < end){
				if(isspace(*begin)){
						begin++;
				}else{
						break;
				}
		}
		while (end > begin){
				if(isspace(*end)){
						end--;
				}else{
						break;
				}
		}
		if( input + len > end){
				*end = '\0';
		}
		return begin;
} /*}}}*/

int format_input(char *input,char *output[],int size) /*{{{*/
{
		int i;
		char *token;			
		char *delim = " ";
		char *saveptr;
		if(input == NULL || output == NULL){
				return -1;
		}
		size--;
		for(i=0;i<size;i++,input=NULL){
				token = strtok_r(input,delim,&saveptr);
				if(token == NULL){
						break;
				}
				output[i] = trim(token,strlen(token));
		}
		output[i] = NULL;
		return 0;
}/*}}}*/

int simple_command(const char *command) /*{{{*/
{
		int ret = 0;
		char buf[1024] ;
		time_t t;
		if(command == NULL){
				return  -1;
		}
		if((*command=='q' && *(command+1)=='\0') || strcmp(command,"quit") == 0){
				ret = 2;	
		}else if(strcmp(command,"pwd") == 0){
				if(getcwd(buf,sizeof(buf)) ==NULL){
						perror("getcwd");
				}
				printf("%s\n",buf);
		}else if(strcmp(command,"now") == 0) {
				t = time(NULL);
				format_time(&t,buf,sizeof(buf));
				printf("%s\n",buf);
		}else{
				ret = -1;
		}
		return ret;
}/*}}}*/

void input(char *buf,int size) /*{{{ */
{
		int len = 0;
		char dir_buf[256]={0};
		if(getcwd(dir_buf,sizeof(dir_buf)) == NULL){
			perror("getcwd");
		}
		do{
				printf("[you shell  %s]# ",basename(dir_buf));
				fgets(buf,size,stdin);
				fflush(stdin);
				len = strlen(buf);
				buf[len-1] = '\0';
				if(len>=2)
					break;
		} while(1);
}/*}}}*/

int main(int argc,char *argv[])
{
		pid_t pid;	
		char *child_argv[32];
		char buf[1024];
		int ret= 0;
		setenv("PATH",".",1);
		do{
				input(buf,sizeof(buf));
				if(format_input(buf,child_argv,sizeof(child_argv)) == -1){
						continue;
				}
				ret = simple_command(child_argv[0]);
				if(ret == 0) { //processed
						continue;
				}
				if(ret == 2) {
						break;
				}
				if(access(child_argv[0],F_OK | X_OK) == -1){
						printf("you shell : %s command not found\n",child_argv[0]);
						continue;
				}
				pid = fork();
				if(pid == 0){
						execvp(child_argv[0],child_argv);
						perror("execv");
						return -1;
				}else{
						wait(NULL);
				}
		} while(1);
		return 0;
}
