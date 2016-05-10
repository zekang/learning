#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define MMAP_FILE "/tmp/_mmap_"
#define MMAP_SIZE 10000
typedef struct _student
{
		char sex;
		int id;
		char name[64];
}student;


void sender() /*{{{*/
{
		int fd ;
		int exists = 1;
		int flag = O_RDWR;
		int i = 0;
		student *stu;
		if(access(MMAP_FILE,F_OK) == -1){
				flag |= O_CREAT;
				exists = 0;
		}
		if(exists){
				fd = open(MMAP_FILE,flag);
		}else{
				umask(0);
				fd = open(MMAP_FILE,flag,0755);	
		}
		if(fd < 0){
				perror("open");
				return ;
		}
		if(exists == 0){
				lseek(fd,MMAP_FILE - 1,SEEK_SET);
				write(fd,(void *)"",1);
		}
		stu = (student *)mmap(NULL,MMAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);	
		if(stu == MAP_FAILED){
				perror("mmap");
				return ;
		}
		for(i = 0;i<100;i++){	
			stu->id = i;
			stu->sex = i % 2 == 0 ? 'm':'f';
			sprintf(stu->name,"student_%d",i);
			printf("id:%d\tsex:%cname:%s\n", stu->id , stu->sex,stu->name);
			sleep(1);
		}
		munmap(stu,MMAP_SIZE);
		close(fd);
		unlink(MMAP_FILE);
}/*}}}*/

void receiver() /*{{{*/
{
		int fd ;
		int exists = 1;
		int flag = O_RDWR;
		int i = 0;
		student *stu;
		if(access(MMAP_FILE,F_OK) == -1){
				flag |= O_CREAT;
				exists = 0;
		}
		if(exists){
				fd = open(MMAP_FILE,flag);
		}else{
				umask(0);
				fd = open(MMAP_FILE,flag,0755);	
		}
		if(fd < 0){
				perror("open");
				return ;
		}
		if(exists == 0){
				printf("Init _mmap_\n");
				lseek(fd,MMAP_FILE - 1,SEEK_SET);
				write(fd,(void *)"",1);
		}
		stu = (student *)mmap(NULL,MMAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);	
		close(fd);
		if(stu == MAP_FAILED){
				perror("mmap");
				return ;
		}
		for(i = 0;i<100;i++){	
			printf("id:%d\tsex:%cname:%s\n", stu->id , stu->sex,stu->name);
			sleep(1);
		}
		munmap(stu,MMAP_SIZE);
}/*}}}*/
int main(int argc,char *argv[])
{
		if(argc < 2){
				printf("usage:mmap sender|receiver\n");
				return 0;
		}
	if(strcmp(argv[1],"sender")==0){
		sender();
	}else if(strcmp(argv[1],"receiver")==0){
		receiver();
	}else{
		printf("error");
	}
	return 0;
}


