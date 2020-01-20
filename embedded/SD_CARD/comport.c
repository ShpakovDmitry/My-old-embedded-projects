#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <time.h>

#define port_name "/dev/ttyUSB0"
#define speed B500000

#define filename "data.txt"


int open_port(void);
void close_port(int fd);
void configure_port(int fd);




int main(int argc, char *argv[])
{
	int fd;
	char *buf;
	char *buf2;
	FILE *f;
	
	f = fopen(filename, "a");
	if (f == NULL)
	{
		printf("ERROR. Can't open file %s\n", filename);
		return -1;
	}
	//*buf = 'c';
	
	printf("fd: %d\n", fd = open_port() );
	
	if(fd == -1)
	{
		printf("Port open ERROR!\n");
		return -1;
	}
	
	configure_port(fd);	
	
	/*while( (*buf) != 'q')
	{
		printf("Enter: ");
		scanf("%s", buf);
		write(fd, buf, 1);
		read(fd, buf, 1);
		printf("%s\n", buf);
	}*/
	
	while (1)
	{
		if( read(fd, buf, 1)  != -1 )
		{
			putc((int)(*buf), f);
			printf("%s", buf);
			fflush(f);
		}
	}
	
	
	/*while(1)
	{
		write(fd, buf, 1);
		sleep(1);
	}*/
	
	close_port(fd);	
	fclose(f);	
	return 0;
}


void configure_port(int fd)
{
	struct termios options;
	
	tcgetattr(fd, &options);
	
	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);
	
	options.c_cflag |= (CLOCAL | CREAD);
	
	
	options.c_cflag &= ~PARENB;	// без проверки чётности
	options.c_cflag &= ~CSTOPB;	// выкл 2-х стоп битб 1 стоп бит
	options.c_cflag &= ~CSIZE;	// выкл битовой маски
	options.c_cflag |= CS8;		// 8 бит данных
	
	
	tcsetattr(fd, TCSANOW, &options);
}



int open_port(void)
{
	int fd;
	
	fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
	if( fd == -1 )
	{
		perror("can't open port");
	}
	else
	{
		fcntl(fd, F_SETFL, 0);
	}
	
	return fd;
}

void close_port(int fd)
{
	close(fd);
}
