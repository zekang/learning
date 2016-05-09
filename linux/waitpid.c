#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>

int main(int argc,char *argv[])
{
		pid_t pid;
		int n = 20	;
		int status;
		int ret ;
		pid = fork();
		printf("child : %d\n",pid);
		if(pid == 0){
				while(n--){
						printf("child %d,parent:%d n:%d\n",getpid(),getppid(),n);
						sleep(1);
				}
		}else{
				do{
						printf("parent wait\n");
						ret = waitpid(pid,&status,WUNTRACED|WCONTINUED);	
						if(ret == -1){
								perror("waitpid");
								return -1;
						}
						if(WIFEXITED(status)){
							printf("child exit with code :%d\n",WEXITSTATUS(status));
						}else if(WIFSIGNALED(status)){
							printf("child exit with signal :%d\n",WTERMSIG(status));
						}else if(WIFSTOPPED(status)){
							printf("child stop with signal:%d\n",WSTOPSIG(status));
						}else if(WIFCONTINUED(status)){
							printf("continue \n");
						}
				}while(!WIFEXITED(status) && !WIFSIGNALED(status));
				printf("parent exit\n");
		}
		return 0;
}
