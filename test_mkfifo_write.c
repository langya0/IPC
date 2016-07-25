#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define _PATH_ "/tmp/file"
#define _SIZE_ 100
int main()
{
	int ret = mkfifo(_PATH_,0777);
	if(ret == -1)
	{
		printf("mkfifo error\n");
		return 1;
	}

	int fd = open(_PATH_, O_WRONLY);

	if(fd < 0)
	{
		printf("open error\n");
	}

	char buf[_SIZE_];
	memset(buf, '\0', sizeof(buf));
	
	while(1)
	{
		scanf("%s", buf);
		int ret = write(fd, buf, strlen(buf));
		if(ret < 0)
		{
			printf("write error\n");
			break;
		}

		if( strncmp(buf, "quit", 4) == 0 )
		{
			break;
		}
	}

	close(fd);
	return 0;
}