#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void do_sig(int signum)
{
	printf("do_sig %d\n",signum);
}

unsigned int mysleep(int second)
{
	unsigned int ret = 0;
	sigset_t set;
	sigemptyset(&set);
	struct sigaction act = {0},oldact;
	act.sa_handler = do_sig;
	act.sa_mask = set;
	act.sa_flags =1;
	if(sigaction(SIGALRM,&act,&oldact) == -1){
		perror("sigaction");
		exit(1);
	}
	if(sigaction(SIGUSR1,&act,&oldact) == -1){
		perror("sigaction");
		exit(1);
	}
	alarm(second);
	pause();
	ret = alarm(0);
	sigaction(SIGALRM,&oldact,NULL);
	return ret;
}

unsigned int mysleep02(int second)
{
	unsigned int ret = 0;	
	sigset_t new_mask,pause_mask,old_mask;
	struct sigaction act = {0},old_act={0};
	
	act.sa_handler = do_sig;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM,&act,&old_act);
	sigaction(SIGUSR1,&act,NULL);
		
	sigemptyset(&new_mask);
	//sigfillset(&new_mask);
	sigaddset(&new_mask,SIGALRM);
	sigprocmask(SIG_BLOCK,&new_mask,&old_mask);

	alarm(second);

	pause_mask = new_mask;
	sigdelset(&pause_mask,SIGALRM);
	
	sigsuspend(&pause_mask);

	ret = alarm(0);	
	sigaction(SIGALRM,&old_act,NULL);
	sigprocmask(SIG_SETMASK,&old_mask,NULL);
	return ret;
}

int main(int argc,char *argv[])
{
	unsigned int ret ;
	while(1){
		ret =mysleep02(5);
		printf("mysleep02 5 seconds %u\n",ret);
	}
	return 0;		
}
