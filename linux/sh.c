#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
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


int main(int argc,char *argv[])
{
		pid_t pid;	
		char *child_argv[32];
		char buf[1024];
		int len;
		int i,j;
		setenv("PATH",".",1);
		do{
				printf("Command:");
				fgets(buf,sizeof(buf),stdin);
				len = strlen(buf);
				buf[len-1] = '\0';
				if(strcasecmp(buf,"quit") == 0){
					break;
				}
				if(len< 2 || (format_input(buf,child_argv,sizeof(child_argv)) == -1)){
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
		}
		while(1);
		return 0;
}
