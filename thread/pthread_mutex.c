#include <stdio.h>
#include <pthread.h>
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int g_val = 0;
void* add(void *argv)
{
	for(int i = 0 ; i < 5000; ++i)
	{
		// g_val++;
		pthread_mutex_lock(&lock);
		int tmp = g_val;
		g_val = tmp+1;
		pthread_mutex_unlock(&lock);
	}
}

int main(int argc, char const *argv[])
{
	pthread_t id1,id2,id3;

	pthread_create(&id1,NULL,add,NULL);
	pthread_create(&id2,NULL,add,NULL);
	pthread_create(&id3,NULL,add,NULL);

	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);

	printf("%d\n",g_val);
	return 0;
}