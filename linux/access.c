#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc ,char *argv[])
{
	int mode,index;
	char *tips[4]={
		"Read",
		"Writer",
		"Execute",
		"Exists"
	};
	if(argc < 3){
		printf("Usage ./access rwxf file\n");
		return -1;
	}
	switch(*argv[1]){
		case 'r':
			mode = R_OK;
			index = 0;
			break;
		 case 'w':
		 	mode = W_OK;
			index = 1;
		 	break;
		case 'x':
			mode = X_OK;
			index = 2;
			break;
		case 'f':
			mode = F_OK;
			index = 3;
			break;
		default :
			index = -1;
			break;
	}
	if(index == -1){
		printf("wrong parameters \n");
		return 0;
	}
	if(access(argv[2],mode)==-1){
		printf("no %s\n",tips[index]);	
	}else{
		printf("have %s\n",tips[index]);	
	}
	return 0;
}
