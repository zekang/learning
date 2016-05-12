#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int i = 0;
	alarm(1);
	while(1){
		printf("i=%d\n",i);
		i++;
	}
	return 0;
}
