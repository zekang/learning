#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>


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
	if(lstat(path,&st) ==  -1){
		perror("stat");
		return;
	}
	format_access(st.st_mode,buf,11);	
	printf(buf);
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
