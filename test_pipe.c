#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
	int _pipe[2] = {0};
	int ret = pipe(_pipe);	//获取管道

	pid_t fd = fork();

	if(fd == -1 )
	{
		perror("fork error");
		return 1;
	}
	else if(fd == 0)
	{
		printf("in child\n");
		close(_pipe[0]);

		const char* msg = "i am child\n";

		int i = 0;
		for(; i < 10; ++i)
		{
			write(_pipe[1],msg,strlen(msg));
		}
		printf("in child\n");

		close(_pipe[1]);
	}
	else
	{
		sleep(1);
		printf("in father\n");
		close(_pipe[1]);

		char recvBuf[100];
		memset(recvBuf,'\0',sizeof(recvBuf));
		while((ret = read(_pipe[0],recvBuf,sizeof(recvBuf)))>0)
		{
			recvBuf[ret] = '\0';
			fprintf(stdout, "%s", recvBuf);
		memset(recvBuf,'\0',sizeof(recvBuf));
		}
		printf("in father\n");
	}

	return 0;
}