#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

struct msg{
	struct msg *next;
	int num;
};

struct msg *head = NULL;
pthread_cond_t has_product  = PTHREAD_COND_INITIALIZER;
pthread_cond_t has_consumer = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock        = PTHREAD_MUTEX_INITIALIZER;

void *product(void *arg)
{
	struct msg *mp;
	for(;;){
		pthread_mutex_lock(&lock);
		while(head != NULL)
			pthread_cond_wait(&has_consumer,&lock);	
		mp = calloc(sizeof(struct msg),1);	
		mp->num = rand() % 1000 + 1;
		printf("Product %d\n",mp->num);
		mp->next = head;
		head = mp;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&has_product);
		sleep(rand() % 4);
	}
}

void *consumer(void *arg)
{
	struct msg *mp;	
	for(;;){
		pthread_mutex_lock(&lock);
		while(head == NULL)
			pthread_cond_wait(&has_product,&lock);
		mp = head;
		head = mp->next;
		pthread_mutex_unlock(&lock);
		printf("Consume %d\n",mp->num);
		free(mp);
		pthread_cond_signal(&has_consumer);
		sleep(rand() % 5);
	}
}

int main(int argc,char *argv[])
{
	srand(time(NULL));	
	pthread_t pid,cid;
	pthread_create(&pid,NULL,product,NULL);
	pthread_create(&cid,NULL,consumer,NULL);
	pthread_join(pid,NULL);
	pthread_join(cid,NULL);
	return 0;
}
