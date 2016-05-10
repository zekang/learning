#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define NLOOP 200
int counter;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER; 
void * th_fun(void *arg)
{
	int i,val;
	for(i=0;i<NLOOP;i++){
		pthread_mutex_lock(&counter_mutex);
		val = counter;
		printf("%x:%d\n",(unsigned int)pthread_self(),val);
		usleep(1000);
		counter = val + 1;
		usleep(1000);
		pthread_mutex_unlock(&counter_mutex);
	}
}

int main(int argc,char *argv[])
{
	pthread_t tid_a,tid_b;
	pthread_create(&tid_a,NULL,th_fun,NULL);
	pthread_create(&tid_b,NULL,th_fun,NULL);

	pthread_join(tid_a,NULL);
	pthread_join(tid_b,NULL);
	return 0;
}
