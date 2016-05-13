#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<fcntl.h>


int main(int argc,char *argv[])
{
	pid_t pid;
	int log_fd,null_fd;
	char buf[1024];
	int len;
	if (argc < 2){
		printf("usage:logd logifle\n");
		return -1;
	}
	if((pid = fork()) < 0){
		perror("fork");
		return -1;
	}
	if(pid > 0){
		//parent exit;
		return 0;
	}
	printf("prepare daemon......\n");
	sleep(10);
	printf("begin daemon......\n");
	setsid();	
	chdir("/");
	umask(0);
	close(STDIN_FILENO);	
	null_fd = open("/dev/null",O_RDWR);
	dup2(null_fd,STDOUT_FILENO);
	dup3(null_fd,STDERR_FILENO);
	log_fd = open(argv[1],O_CREAT|O_WRONLY);
	while(1){
		len = sprintf(buf,"time %d\n",time(NULL));	
		write(log_fd,buf,len);
		sleep(10);
	}	
	close(log_fd);
	return 0;
}
