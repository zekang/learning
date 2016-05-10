#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *th_fun(void *arg)
{
	int n = 20;
	while(n--){
		printf("%x %d\n",(int)pthread_self(),n);
		sleep(1);
	}
	return (void *)1;
}

int main(int argc,char *argv)
{
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);	
	pthread_create(&tid,&attr,th_fun,NULL);

	int err = pthread_join(tid,NULL);
	while(1){
		if(err != 0){
			printf("%s\n",strerror(err));
			sleep(10);
			pthread_exit((void *)1);
		}
	}
	return 0;
}
