#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
void *product(void *args)
{
	printf("here in product pid = %d pthread_id = %llu\n",(int)getpid(),(unsigned long long)pthread_self());
	pthread_exit(9);
	// exit(2);//自己终止
}
void *consumer(void *args)
{
	// pthread_detach(pthread_self());	////线程分离方式1
	printf("here in consumer pid = %d pthread_id = %llu\n",(int)getpid(),(unsigned long long)pthread_self());
	sleep(3);
	printf("consumer finish\n");
	return;//线程内返回
}
int main(int argc, char const *argv[])
{
	pthread_t id1,id2;
	pthread_create(&id1,NULL,product,NULL);
	pthread_create(&id2,NULL,consumer,NULL);
	// pthread_cancel(id2);///主线程终止从线程
	pthread_join(id1,NULL);
	pthread_detach(id2);	//线程分离方式2
	return 0;
}