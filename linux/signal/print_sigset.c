#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>


int main(int argc,char *argv[])
{
	sigset_t set,old_set,now_set;
	int i = 0,j=0;
	printf("sizeof(sigset_t):%d\n",sizeof(sigset_t));
	sigemptyset(&set);	
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGTSTP);
	sigprocmask(SIG_BLOCK,&set,&old_set);
	while(1){
		sigpending(&now_set);
		for(i=0;i<32;i++){
			if(sigismember(&now_set,i) == 1	){
				putchar('1');
			}else{
				putchar('0');
			}
		}
		putchar(10);
		sleep(1);
		j++;
		if(j>30){
				break;
		}
	}
	sigprocmask(SIG_SETMASK,&old_set,NULL);
	printf("never reached \n");
	return  0;
}
