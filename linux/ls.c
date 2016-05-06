#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<dirent.h>
#include "lib.h"

void display_stat(struct stat *pst,const char *path)/*{{{*/
{
	struct stat st;
	char buf[1024];
	struct tm mytm;
	st = *pst;
	format_access(st.st_mode,buf,12);	
	printf(buf);
	printf(" %d",st.st_nlink);
	printf(" ");
	if(get_name(TYPE_USERNAME,st.st_uid,buf,sizeof(buf))==0){
		printf("%s ",buf);
	}
	if(get_name(TYPE_GROUPNAME,st.st_gid,buf,sizeof(buf))==0){
		printf("%s ",buf);
	}
	printf("%5d ",st.st_size);
	localtime_r(&st.st_mtime,&mytm);
	strftime(buf,sizeof(buf),"%b %d %H:%M",&mytm);
	printf("%s %s",buf,path);
	printf("\n");
}/*}}}*/

void dir_walk(const char *path)/*{{{*/
{
	struct stat st;
	char path_buf[256];
	DIR *dir;
	struct dirent *entry;
	if(lstat(path,&st) == -1){
		perror("lstat");
		return;
	}
	if(S_ISDIR(st.st_mode)){
		dir = opendir(path);		
		if(dir == NULL){
			perror("opendir");
			return;
		}
		while ((entry = readdir(dir)) != NULL){
			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
				continue;
			}
			if(path[strlen(path)-1] != '/'){
					sprintf(path_buf,"%s/%s",path,entry->d_name);
			}else{
					sprintf(path_buf,"%s%s",path,entry->d_name);
			}
			dir_walk(path_buf);
		}
		closedir(dir);
	}
	if(strcmp(path,".")!=0 && strcmp(path,"..")!=0){
			display_stat(&st,path);
	}
}/*}}}*/



int main(int argc,char *argv[])
{
	int i ;
	if(argc == 1){
		argc = 2;
		argv[1] = ".";
	}
	for(i = 1;i<argc;i++){
			dir_walk(argv[i]);
	}
	return 0;
}
