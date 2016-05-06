#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<dirent.h>
#include "lib.h"

int file_read_line(int fd,char *buf/*output*/,int size) /*{{{*/
{
	int ret = -1,index = 0;
	int len;
	char tmp;
	do{
			len = read(fd,&tmp,1);
			if(len == -1){
				perror("read");
				break;
			}
			buf[index++] = tmp;
			if(len ==0 || index >= size - 1 || tmp=='\n'){
				buf[index] = '\0';
				ret = 0;
				break;
			}
	}while(1);
	return ret;
}/*}}}*/

int parse_line(char *line,char **output,int size) /*{{{*/
{
	int ret = -1,i;
	char *token,*delim=":",*saveptr;
	for(i=0;i<size;line=NULL,i++){
		token = strtok_r(line,delim,&saveptr);
		output[i] = token;
	}
	ret = i;
	return ret;
}/*}}}*/

int get_name(int type,int id,char *output ,int size) /*{{{*/
{
	int ret = -1;
	int fd;
	char line_buf[4096];
	char *info[8] = {0};
	if(type == TYPE_USERNAME){
			fd = open(FILE_PASSWD,O_RDWR);
	}else if( type == TYPE_GROUPNAME){
			fd = open(FILE_GROUP,O_RDWR);
	}else{
		fprintf(stderr,"bad type value\n");
		return ret;
	}
	if(fd < 0){
		perror("open");
		return ret;
	}
	do {
		ret = file_read_line(fd,line_buf,sizeof(line_buf));
		if(ret == -1) {
				break;
		}
		memset(info,0,sizeof(info));
		ret = parse_line(line_buf,info,sizeof(info)/sizeof(*info));
		if(ret > 2){
			if( atoi(info[2]) == id && info[0]) {
				strncpy(output,info[0],size);
				ret = 0;
				break;
			}
		}
	}while(1);
	if(fd>0){
		close(fd);
	}
	return ret;	
}/*}}}*/

int format_access(mode_t mode,char *buf /*output*/,int count)/*{{{*/
{
	if(buf == NULL || count < 11){
		return -1;
	}
	memset(buf,'-',count);	
	buf[count-2]='.';	
	buf[count-1]='\0';	
	if(S_ISDIR(mode)){
		buf[0] = 'd';
	}else if(S_ISLNK(mode)){
		buf[0] = 'l';
	}else if(S_ISSOCK(mode)){
		buf[0] = 's';
	}
	if(mode & S_IRUSR){
		buf[1] = 'r';
	}
	if(mode & S_IWUSR){
		buf[2] = 'w';
	}
	if(mode & S_IXUSR){
		buf[3] = 'x';
	}

	if(mode & S_IRGRP){
		buf[4] = 'r';
	}
	if(mode & S_IWGRP){
		buf[5] = 'w';
	}
	if(mode & S_IXGRP){
		buf[6] = 'x';
	}

	if(mode & S_IROTH){
		buf[7] = 'r';
	}
	if(mode & S_IWOTH){
		buf[8] = 'w';
	}
	if(mode & S_IXOTH){
		buf[9] = 'x';
	}
	return 0;
}/*}}}*/

int format_access_as_number(mode_t mode,char *buf /*output*/,int count)/*{{{*/
{
	int tmp = 0;
	if(buf == NULL || count < 5){
		return -1;
	}
	buf[0] = '0';
	buf[count-1]='\0';	
	
	if(mode & S_IRUSR){
		tmp += 4;
	}
	if(mode & S_IWUSR){
		tmp += 2;
	}
	if(mode & S_IXUSR){
		tmp += 1;
	}
	buf[1] = tmp + '0';
	tmp = 0;
	if(mode & S_IRGRP){
		tmp += 4;
	}
	if(mode & S_IWGRP){
		tmp += 2;
	}
	if(mode & S_IXGRP){
		tmp += 1;
	}
	buf[2] = tmp + '0';
	tmp = 0;
	if(mode & S_IROTH){
		tmp += 4;
	}
	if(mode & S_IWOTH){
		tmp += 2;
	}
	if(mode & S_IXOTH){
		tmp += 1;
	}
	buf[3] = tmp + '0';
	tmp = 0;
	return 0;
}/*}}}*/

int format_time(time_t *timep,char *buf /*output*/,int count) /*{{{*/
{
	struct tm _tm;
	if(timep== NULL || buf == NULL || count < 1){
		return -1;
	}
	if(localtime_r(timep,&_tm) == NULL){
		perror("localtime_r");
		return -1;
	}
	strftime(buf,count,"%Y-%m-%d %H:%M:%S",&_tm);
	return 0;
}/*}}}*/
