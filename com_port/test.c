#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <time.h>

#define port_name "/dev/ttyUSB0"
#define speed B500000
//#define speed B2400
//#define speed B1500000



#define BUFSIZE 2 


int open_port(void);
void close_port(int fd);
void configure_port(int fd);





int main(int argc, char *argv[])
{
	int fd;
	int n;
	int i;
	int ch;
	char buf[BUFSIZE];
	char c;
	long int k;
	unsigned int x;
	//struct timespec tw = {0, 1};
	//struct timespec tr;
	
	printf("fd: %d\n", fd = open_port() );
	
	if(fd == -1)
	{
		printf("Error. Can't open port\n");
		return 1;
	}
	
	configure_port(fd);	
	
	FILE *f = fopen("card.txt", "wb");
	if(f == NULL)
	{
		printf("error\n");
		return -1;
	}
	
	/*while( (ch = getc(f)) != EOF )
	{
		(*buf) = (char)ch;
		write(fd, buf, 1);
		//usleep(3);
		//nanosleep(&tw,&tr);
		//
		//printf("i'm here\n");
		//for(k=0;k<2e4;k++)
		//	;
	}*/
	
	
	
	//n = write(fd, argv[1], 1);
	//printf("writed %d bytes\n", n);
	
	i = 0;
	while(1)
	{
		/*while( read(fd, buf, BUFSIZE) == 0 )
		{
			;
		}*/
		//sleep(1);
		read(fd, buf, BUFSIZE);
		
		fprintf(stdout,"%s", buf);
		//printf("0x%c%c\t", ((unsigned char)(*buf)/16 > 9) ? ((unsigned char)(*buf)/16 - 10 + 'a') : ((unsigned char)(*buf)/16 + '0'),\
				   ((unsigned char)(*buf)%16 > 9) ? ((unsigned char)(*buf)%16 - 10 + 'a') : ((unsigned char)(*buf)%16 + '0'));
		//printf("data: %s, i:%d\n",buf, i);
		//i++;
		fprintf(f, "%s", buf);
		fflush(f);
	
	}
	
	fclose(f);
	close_port(fd);		
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
