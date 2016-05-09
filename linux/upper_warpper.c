#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


int main(int argc,char **argv)
{
	int fd;
	if(argc < 2){
		printf("usage:upper_wrapper file\n");
		return -1;
	}
	if(access(argv[1],F_OK)!=0){
		printf("%s is not exists \n",argv[1]);
		return -1;
	}
	fd = open(argv[1],O_RDONLY);	
	if(fd < 0){
		perror("open");
		return -1;
	}
	dup2(fd,STDIN_FILENO);
	close(fd);
	execl("./upper","upper",NULL);
	perror("exec ./upper");
	return -1;	
}
