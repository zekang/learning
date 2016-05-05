#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>

int main(int argc,char **argv)
{
	int mode ;
	if(argc < 3){
		printf("usage ./chmode path mode\n");
		return 0;
	}
	mode = strtol(argv[2],NULL,8);
	if(chmod(argv[1],mode)==-1){
		perror("chmod");
		return -1;
	}
	return 0;
}
