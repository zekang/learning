#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{	
		pid_t pid = 0;	
		int filefd[2] ; 
		char buf[1024];
		int n,i;
		if(pipe(filefd) == -1){
				perror("pipe");
				return -1;
		}
		pid = fork();
		if(pid < 0){
				perror("fork");
				exit(1);
		}
		//child write parent read
		//filefd[0] == STDIN_FILENO  input
		//filefd[1] == STDOUT_FILENO output
		if(pid == 0){
				close(filefd[0]);
				for(i=0;i<20;i++){
						write(filefd[1],"hello world\n",12);
						sleep(1);
				}
				close(filefd[1]);
		}else{
				close(filefd[1]);	
				i = 0;
				do{
						i++;
						printf("%d\n",i);
						n = read(filefd[0],buf,sizeof(buf));
						if(n<0){
							perror("read");
							break;
						}
						write(STDOUT_FILENO,buf,n);
						sleep(1);
				} while(i<11);
				close(filefd[0]);
				sleep(15);
				wait(NULL);
		}
		return 0;	
}
