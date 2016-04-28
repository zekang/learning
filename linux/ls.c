#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>

#define TYPE_USERNAME 1
#define TYPE_GROUPNAME 2
#define FILE_PASSWD "/etc/passwd"
#define FILE_GROUP  "/etc/group"

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

void display_file(const char *path)/*{{{*/
{
	struct stat st;
	char buf[1024];
	struct tm mytm;
	if(lstat(path,&st) ==  -1){
		perror("stat");
		return;
	}
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




int main(int argc,char *argv[])
{
	int i ;
	for(i = 1;i<argc;i++){
			display_file(argv[i]);
	}
	return 0;
}
