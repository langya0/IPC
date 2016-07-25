#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t lock;

int g_val = 0;
void *reader(void*args)
{
	while(1)
	{
		sleep(1);
		if(pthread_rwlock_rdlock(&lock)!=0)
		{
			printf("reader waiting...\n");
		}
		else
		{
			printf("%s,g_val = %d\n",__func__,g_val);
			pthread_rwlock_unlock(&lock);
		}
	}
}
//读写互斥，读者可重入，写着互斥不可重入。
void *writer(void*args)
{
	while(1)
	{
		sleep(1);
		if(pthread_rwlock_wrlock(&lock)!=0)
		{
			printf("writer waiting...\n");
		}
		else
		{
			g_val++;
			printf("in %s g_val = %d \n",__func__, g_val);
			pthread_rwlock_unlock(&lock);
		}
	}
}
int main(int argc, char const *argv[])
{
	pthread_rwlock_init(&lock,NULL);

	pthread_t read,writ;
	for(int i = 0 ; i < 5; ++i)
	{
		pthread_create(&read,NULL,reader,NULL);
	}
	for(int i = 0; i < 5; ++i)
	{
		pthread_create(&writ,NULL,writer,NULL);
	}

	sleep(20);
	pthread_join(read,NULL);
	pthread_join(writ,NULL);

	return 0;
}