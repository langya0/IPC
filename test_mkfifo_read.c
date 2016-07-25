#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define _PATH_ "/tmp/file"
#define _SIZE_ 100

int main()
{
	int fd = open(_PATH_,O_RDONLY);
	if(fd < 0)
	{
		fprintf(stderr, "%s\n","open error");
		return 1;
	}

	char Buff[_SIZE_];
	memset(Buff,0,sizeof(Buff));

	while(1)
	{
		int ret = read(fd,Buff,sizeof(Buff));
		if(ret < 0)
		{
			fprintf(stderr, "%s\n","read error");
		}
		printf("%s\n", Buff);

		if( strncmp(Buff, "quit", 4) == 0 )
		{
			break;
		}
	}
	close(fd);

	return 0;
}