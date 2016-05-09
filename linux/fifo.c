#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define FIFO_NAME "/tmp/_fifo_"


void sender()
{
	int fd;	
	int flag = O_WRONLY;
	int n = 0;
	int len;
	char buf[64];
	if(access(FIFO_NAME,F_OK) != 0){
		flag |= O_CREAT;
	}
	fd = open(FIFO_NAME,flag);
	if(fd < 0){
		perror("open");
		return;
	}
	for(n=0;n<30;n++){
		len = sprintf(buf,"hello world %d\n",n);
		write(fd,buf,len);
		printf("writing(%d) %s\n",len,buf);
		sleep(1);
	}
	write(fd,"quit",sizeof("quit")-1);
	close(fd);
	unlink(FIFO_NAME);
}

void receiver()
{
	int fd;	
	int flag = O_RDONLY;
	char buf[128]={0};
	int len;
	if(access(FIFO_NAME,F_OK) != 0){
		flag |= O_CREAT;
	}
	fd = open(FIFO_NAME,flag);
	if(fd < 0){
		perror("open");
		return;
	}
	do{
		len = read(fd,buf,sizeof(buf));	
		if(len < 0){
			perror("read");
			break;
		}
		buf[len]= '\0';
		if((len == 4) && strcmp("quit",buf) == 0){
			break;
		}
		printf("read(%d) : %s\n",len,buf);
		sleep(1);
	}while(1);
	close(fd);
}

int main(int argc,char *argv[])
{	
	if(argc<2){
		printf("useage:fifo sender|receiver\n");
		return -1;
	}
	if(strcmp(argv[1],"sender") == 0){
		sender();
	}else if(strcmp(argv[1],"receiver") == 0){
		receiver();
	}else{
		printf("wrong param\n");
	}
	return 0;
}
