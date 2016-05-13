#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	pid_t pid;
	if ( (pid = fork()) < 0) {
		perror("fork");
		return -1;
	}
	if (pid == 0) {
		printf("child id : %d\n",getpid());		
		printf("parent id :%d\n",getppid());
		printf("group id:%d\n",getpgid(0));
		printf("session id:%d\n",getsid(0));
		sleep(6);
		setsid();
		printf("child id : %d\n",getpid());		
		printf("parent id :%d\n",getppid());
		printf("group id:%d\n",getpgid(0));
		printf("session id:%d\n",getsid(0));
		sleep(10);
		return 0;
	}
	/**
	printf("parent session:%d\n",getsid(getpid()));
	printf("parent group:%d\n",getpgid(getpid()));
	wait(NULL);
	*/
	return 0;
}
