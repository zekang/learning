#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<string.h>


int main(int argc,char *argv[])
{	

	int fd;
	char *result;
	if(argc < 2){
		printf("usage:mmap file\n");
		return 0;
	}
	umask(0);
	fd = open(argv[1],O_CREAT|O_RDWR,0755);	
	if(fd<0){
		perror("open");
		goto END;
	}
	if(lseek(fd,1000 - 1,SEEK_SET)==-1){
		perror("lseek");
		goto END;
	}
	write(fd,(void *)"",1);
	result = (char *)mmap(NULL,1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(result == MAP_FAILED) {
		perror("mmap");
		goto END;
	}
	close(fd);
	strcpy(result,"hello world\n");
	strcat(result,"learning linux\n");
	puts(result);
	if(munmap(result,1000)==-1){
		perror("munmap");
		goto END;
	}
END:
	return 0;
}
