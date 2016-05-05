#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TYPE_USER 1
#define TYPE_GROUP 2

#define USER_FILE  "/etc/passwd"
#define GROUP_FILE "/etc/group" 

int get_id(int type,char *name /*in*/,int name_len , char *buf/*out*/,int size) /*{{{*/
{		
	FILE *fp = NULL;
	int ret = -1;
	char line[1024];
	char *start,*end;
	int i;
	if(name_len == -1){
		name_len = strlen(name);
	}
	if(type == TYPE_USER) {
		fp = fopen(USER_FILE,"r");
	} else if(type == TYPE_GROUP) {
		fp = fopen(GROUP_FILE,"r");
	}
	if(fp == NULL){
		perror("fopen");
		return ret;
	}
	do{
		if(fgets(line,sizeof(line),fp) ==NULL){
			break;
		}
		if(strncmp(line,name,name_len)){
			continue;
		}
		start = line;
		for(i=0;i<2;i++){
			start = strchr(start,':');
			start++;
		}
		if(start && start != line){
			end = strchr(start,':');
			i = end - start;	
			i = i > size - 1 ? size : i;
			memcpy(buf,start,i);
			buf[i] = '\0';
			ret=0;
		}
	}while(1);
	fclose(fp);
	return ret;
}/*}}}*/


int main(int argc,char *argv[])
{
	int uid = -1;
	int gid = -1;
	char *pos;
	char buf[32];
	if(argc < 3){
		printf("Usage chown owner:group file\n");
		return 0;
	}

	pos = strchr(argv[1],':');	
	if(pos != NULL){
		if(get_id(TYPE_USER,argv[1],pos-argv[1],buf,sizeof(buf)) != 0){
			printf("user %s not exists\n",argv[1]);
			return -1;
		}
		uid = atoi(buf);
		if(get_id(TYPE_GROUP,pos+1,-1,buf,sizeof(buf)) != 0){
			printf("group %s not exists\n",pos+1);
			return -1;
		}
		gid = atoi(buf);
	}else{
		if(get_id(TYPE_USER,argv[1],-1,buf,sizeof(buf)) != 0){
			printf("user %s not exists\n",argv[1]);
			return -1;
		}
		uid = atoi(buf);
	}
	printf("uid=%d,gid=%d\n",uid,gid);
	if(chown(argv[2],uid,gid)==-1){
		perror("chown");
		return -1;
	}
	return 0;
}
