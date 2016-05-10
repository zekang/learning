#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

void *th_fun(void *arg)
{
	while(1)
		sleep(3);
}

int main(void)
{
	pthread_t tid;
	int i=1,err;
	while(1){
		err = pthread_create(&tid,NULL,th_fun,NULL);
		if(err !=0){
			fprintf(stderr,"%s\n",strerror(err));
			exit(1);
		}
		printf("%d\n",i++);
	}
}
