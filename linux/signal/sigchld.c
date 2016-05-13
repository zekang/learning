#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

void do_sig_child(int signum)
{
	pid_t pid;
	int status;
	pid = waitpid(-1,&status,0);	
	if(pid == -1){
		perror("waitpid");
		return;
	}
	if(WIFEXITED(status)){
		printf("child %d quit,with status %d\n ",pid,WEXITSTATUS(status));
	}else if(WIFSIGNALED(status)){
		printf("child %d quit with signal %d\n",pid,WTERMSIG(status));
	}
}

void do_sig_child_02(int signum)
{
	pid_t pid;	
	int status;
	while((pid = waitpid(-1,&status,WNOHANG)) != -1){
		if(WIFEXITED(status)){
			printf("child %d quit with status %d\n",pid,WEXITSTATUS(status));
		}else if(WIFSIGNALED(status)){
			printf("child %d quit with signal %d\n",pid,WTERMSIG(status));
		}else {
			printf("child %d exit\n");
		}
	}
}

int main(int argc,char *argv[])
{
		int i = 0,j;
		pid_t pid;
		struct sigaction act;
		while (i<10) {
				if ((pid=fork()) == 0){
						break;
				}else if(pid < 0){
						perror("fork");
						exit(1);
				}
				i++;
		}
		if(pid == 0){
			for(j=0;j<30;j++){
				printf("j=%d,child:%d,parent:%d \n",j,getpid(),getppid());
				sleep(1);
			}
			return i;
		}else{
			bzero(&act,sizeof(struct sigaction));	
			//act.sa_handler = do_sig_child;
			act.sa_handler = do_sig_child_02;
			sigemptyset(&act.sa_mask);
			act.sa_flags = 0;
			sigaction(SIGCHLD,&act,NULL);
			while(1){
				sleep(1);
			}
		}
		return 0;
}
