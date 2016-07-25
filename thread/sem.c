#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h>
#include <pthread.h>
#define _SIZE_ 128

int buf[_SIZE_];
sem_t blanks;
sem_t datas;

//生产者
void *producter(void *val)
{
	int beg = 0;
	while(1)
	{
		sem_wait(&blanks);
		int data = rand()%1024;
		buf[beg] = data;

		printf("%s done... data = %d\n",__func__,data);
		sem_post(&datas);
		beg = (beg+1)%_SIZE_;
		sleep(2);
	}
	return NULL;
}

//消费者
void *consumer(void *val)
{
	int start = 0;
	while(1)
	{
		sem_wait(&datas);
		int data = buf[start];

		printf("%s dene... data = %d lwpid=%llu\n", __func__,data,(unsigned long long)pthread_self());
		sem_post(&blanks);
		start = (start+1)%_SIZE_;
		sleep(2);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	sem_init(&blanks,0,_SIZE_);
	sem_init(&datas,0,0);

	pthread_t id1,id2,id3,id4;
	pthread_create(&id1,NULL,producter,NULL);
	pthread_create(&id3,NULL,consumer,NULL);
	pthread_create(&id2,NULL,consumer,NULL);
	// pthread_create(&id4,NULL,producter,NULL);

	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	// pthread_join(id4,NULL);

	sem_destroy(&blanks);
	sem_destroy(&datas);
	return 0;
}