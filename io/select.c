#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int fd_num[64];
const int sock_len = sizeof(fd_num)/sizeof(fd_num[0]);

static int startup(const char* ip,const int port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM,0);
	if(listen_sock<0)
	{
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		exit(3);
	}
	if(listen(listen_sock,5)<0)
	{
		perror("listen");
		exit(4);
	}
	return	listen_sock;
}
static void usage(const char * proc)
{
	printf("Usage: %s [ip] [port]\n",proc);
}

int main(int argc, char const *argv[])
{
	if(argc	!= 3)
	{
		usage(argv[0]);
		exit(1);
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));

	//初始化为空
	for(int i =0;i<sock_len;++i)
	{
		fd_num[i] = -1;
	}

	fd_set rset;//读
	FD_ZERO(&rset);
	FD_SET(listen_sock,&rset);
	fd_num[0]=listen_sock;

	int done = 0;
	while(!done)
	{
		int max_fd = -1;
		//获取最大文件描述符值
		for (int i = 0; i < sock_len; ++i)
		{
			if (fd_num[i]>0)
			{
				FD_SET(fd_num[i],&rset);//设置监听事件给读
				max_fd = max_fd<fd_num[i]?fd_num[i]:max_fd;
			}
		}

		struct timeval timeout = {5,0};
		switch(select(max_fd+1,&rset,NULL,NULL,NULL))
		{
			case 0:
				printf("timeout...\n");
				break;
			case -1:
				perror("select");
				break;
			default:
			{
				int i = 0;
				for(;i < sock_len;++i)
				{
					if(i==0&&FD_ISSET(listen_sock,&rset))
					{
						struct sockaddr_in perr;
						socklen_t len = sizeof(perr);
						int new_fd = accept(listen_sock,(struct sockaddr*)&perr,&len);
						if(new_fd > 0)
						{
							printf("get a new client : socket -> %s:%d\n",inet_ntoa(perr.sin_addr),ntohs(perr.sin_port));
							int j = 0;
							for(;j < sock_len;++j)	
							{
								if(fd_num[j]==-1)
								{
									fd_num[j] = new_fd;
									break;
								}
							}//for

							if(j == sock_len)
							{
								close(new_fd);
							}
						}//if
					}
					else
					{
						if(FD_ISSET(fd_num[i],&rset))
						{
							char buf[1024];
							memset(buf,'\0',sizeof(buf));
							ssize_t _s = read(fd_num[i],buf,sizeof(buf)-1);
							if(_s>0)
							{
								printf("client# %s\n",buf );
							}
							else if(_s == 0)
							{
								printf("client close...\n");
								close(fd_num[i]);
								fd_num[i] = -1;
							}
							else
							{
								perror("read");
							}
						}
					}
				}
			}
				break;
		}
	}
	return 0;
}