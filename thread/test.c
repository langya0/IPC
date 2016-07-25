#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int g_val = 10;
void * test1(void* args)
{
	g_val = 20;
	printf("in %s: g_val = %d\n",__func__, g_val);
}
void * test2(void* args)
{
	sleep(1);
	printf("in %s: g_val = %d\n",__func__,g_val);
}
int main(int argc, char const *argv[])
{
	pthread_t id1,id2;
	pthread_create(&id1,NULL,test1,NULL);
	pthread_create(&id2,NULL,test2,NULL);
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	return 0;
}