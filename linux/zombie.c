#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char **argv)
{
	pid_t pid;
	pid = fork();
	if(pid>0){
		while(1){
			sleep(1);
		}
	}else{
		printf("child is running\n");
		sleep(10);	
	}
	return 0;
}
