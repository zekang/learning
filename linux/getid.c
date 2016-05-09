#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
	uid_t u1,u2;
	pid_t p1,p2;
	u1 = getuid();
	u2 = geteuid();
	p1 = getpid();
	p2 = getppid();
	printf("uid=%d,euid=%d,pid=%d,parend pid = %d\n",u1,u2,p1,p2);
	return 0;
}
