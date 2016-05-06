#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include "lib.h"

void display_stat(const char *path,struct stat *st) /*{{{*/
{
	char buf[128];
	printf("  File:`%s`\n",path);	
	printf("  Size:%d\t\tBlocks:%d\t\tIO Blocks:%d\n",st->st_size,st->st_blocks,st->st_blksize);	
	printf("Device:%xh/",st->st_dev);	
	printf("%dd",st->st_dev);	
	printf("\tInode:%d\t\tLinks:%d\n",st->st_ino,st->st_nlink);	
	printf("Access: (",buf);
	if(format_access_as_number(st->st_mode,buf,5) == 0){
		printf("%s",buf);
	}
	if(format_access(st->st_mode,buf,12) == 0){
		buf[10] = '\0';
		printf("/%s ",buf);
	}
	get_name(TYPE_USERNAME,st->st_uid,buf,sizeof(buf));
	printf(")  Uid:(%5d/  %s)",st->st_uid,buf);

	get_name(TYPE_GROUPNAME,st->st_gid,buf,sizeof(buf));
	printf("  Gid:(%5d/  %s)",st->st_gid,buf);
	printf("\n");
	if(format_time(&st->st_atime,buf,sizeof(buf)) == 0){	
			printf("Access:%s\n",buf);
	}
	if(format_time(&st->st_mtime,buf,sizeof(buf)) == 0){	
			printf("Modify:%s\n",buf);
	}
	if(format_time(&st->st_ctime,buf,sizeof(buf)) == 0){	
			printf("Change:%s\n",buf);
	}
}/*}}}*/


int main(int argc ,char *argv[])
{
	struct stat st;
	if(argc < 2) {
		printf("./stat file\n");
		return -1;
	}
	if(stat(argv[1],&st) == -1){
		perror("stat");
		return -1;
	}
	display_stat(argv[1],&st);
	return 0;
}
