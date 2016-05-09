#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<unistd.h>

int main(int argc,char **argv[])
{
	pid_t pid;
	pid = fork();
	int status;

	if(pid > 0){
		printf("wait child \n");
		wait(&status);
		printf("wait child success %d\n",WEXITSTATUS(status));
		sleep(5);
	}else{
		printf("child is runing\n");
		sleep(10);
		printf("child is ending\n");
		return 2;
	}
	return 0;
}
