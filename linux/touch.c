#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<utime.h>
#include<unistd.h>


int main(int argc,char *argv[])
{
		int fd;
		if(argc < 2){
				printf("Usage:./touch file\n");
				return 0;
		}
		if(access(argv[1],F_OK)==0){
				if(utime(argv[1],NULL) == -1){
						perror("utime");
						return -1;
				}
		}else{
			fd = open(argv[1],O_CREAT,0644);
			if(fd < 0){
				perror("open");
				return -1;
			}
			close(fd);
		}
		return 0;
}
