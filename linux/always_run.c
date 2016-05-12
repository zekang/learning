#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
	int i = 0;
	while(i<1000000){
		fprintf(stderr,"id:%d\n",i);
		i++;
		sleep(1);
	}
	return 0;
}
