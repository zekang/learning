#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int ci = 0,pi = 1;

void child_sa_handler(int signum)
{
	printf("child=%d\n",ci);
	ci += 2;
	kill(getppid(),SIGUSR2);
}
void parent_sa_handler(int signum)
{
	printf("parent=%d\n",pi);
	pi += 2;	
}



int main(int argc,char *argv[])
{
	 pid_t pid;
	 struct sigaction sig_child ={0},sig_parent={0};
	 int i = 0;
	 sig_child.sa_handler = child_sa_handler;
	 sig_child.sa_flags = 0;
	 sig_parent.sa_handler = parent_sa_handler;
	 sig_parent.sa_flags = 0;
	 sigaction(SIGUSR1,&sig_child,NULL);
	 sigaction(SIGUSR2,&sig_parent,NULL);
	 pid = fork();
	 if(pid == 0){ //child
	 	 while(i<100){
			sleep(1);
		 	i++;
		 }
	 }else{
	 	 while(i<100){
			sleep(1);
		 	kill(pid,SIGUSR1);
		 	i++;
			printf("\n");
		 }
		 wait(NULL);
	 }
	 return 0;
}
