#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{

		printf("STDIN_FILENO(%d):%s\n",STDIN_FILENO,ttyname(STDIN_FILENO));
		printf("STDOUT_FILENO(%d):%s\n",STDOUT_FILENO,ttyname(STDOUT_FILENO));
		printf("STDERR_FILENO(%d):%s\n",STDERR_FILENO,ttyname(STDERR_FILENO));
		return 0;
}
