#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
		pid_t pid;

		pid = fork();

		if(pid == 0){
			printf("child id = %d\n",getpid());
			printf("parent id =%d\n",getppid());
			printf("group id=%d\n",getpgrp());
			printf("group id=%d\n",getpgid(0));
			printf("group id=%d\n",getpgid(getpid()));
			return 0;
		}else if(pid >0){
			sleep(3);
			printf("parent id:%d\n",getpid());
			printf("group id :%d\n",getpgrp());

		}else{
		}


		return 0;
}
