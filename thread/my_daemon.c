#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

void my_daemon()
{
	int i;
	int fd0;
	pid_t pid;
	struct sigaction sa;
	umask(0); //设置文文件掩码为0
	if( (pid = fork()) < 0 )
	{

	}else if (pid != 0)
	{
		exit(0); //终止止父父进程
	}
	setsid(); //设置新会话
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if( sigaction(SIGCHLD, &sa, NULL ) < 0 )
	{ // 注册子子进程退出忽略信号
		return;
	}
	if( (pid = fork())<0)
	{
	 //再次fork,终止止父父进程,保持子子进程不是话首首进程,从而而保证后续	不会在和其他终端关联
		printf("fork error!\n");
		return;
	}else if( pid != 0)
	{
		exit(0);
	}
	if( chdir("/") < 0 )
	{//更改工工作目目录到根
		printf("child dir error\n");
		return;
	}
	close(0);
	fd0 = open("/dev/null", O_RDWR); // 关闭标准输入入,重定向所有标准(输入入输出错误)	到/dev/null
	dup2(fd0, 1);
	dup2(fd0, 2);
}
int main(int argc, char const *argv[])
{
	my_daemon();
	while(1)
		sleep(1);
	return 0;
}

#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	daemon(0,0);
	while(1);
	return 0;
}