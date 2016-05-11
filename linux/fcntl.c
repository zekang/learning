#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc,char *argv[])
{
		char buf[1024];
		int len;
		int flag = fcntl(STDIN_FILENO,F_GETFL);
		printf("%d\n",flag);
		flag |= O_NONBLOCK;
		printf("%d\n",flag);
		if(fcntl(STDIN_FILENO,F_SETFL,flag) == -1){
				perror("fcntl");
				exit(1);
		}
		do{
				int len = read(STDIN_FILENO,buf,sizeof(buf));
				if(len>0){
						buf[len-1]='\0';
						printf("read(%d):%s\n",len,buf);
				}else{
					if(errno != EAGAIN){
						perror("read");
						exit(1);
					}
					printf("%d\n",errno);
				}
				sleep(1);
		}while(1);
		return 0;
}
