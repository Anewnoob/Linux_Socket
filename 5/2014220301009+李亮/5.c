#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>

#define BUFFSIZE 20
int main()
{
	int fd;
	fd=open("/dev/ttyS0",O_RDWR);
	if(fd==-1)
		perror("open ttyS0 failed\n");
	struct termios opt;		//to put ttyS0 info
	tcgetattr(fd,&opt);		//get the ttyS0 set info
	cfsetispeed(&opt,9600);		//set in speed
	cfsetospeed(&opt,9600);	
	opt.c_cflag &=~CSTOPB;		//set  1 support
	opt.c_cflag &=~PARENB;		//no check
	opt.c_cflag |=CS8;		//8 data bits
	tcsetattr(fd,TCSANOW,&opt);
	
	char send[BUFFSIZE];
	char recv[BUFFSIZE];
	while(1){
		printf("Send:");
		memset(send,0,sizeof(send));
		gets(send);
		if(strcmp(send,"exit")==0)	
		{
			break;
		}
		int len=strlen(send);
		write(fd,send,len);

		memset(recv,0,sizeof(recv));
		int m=read(fd,recv,BUFFSIZE);
		if(m>0)
			printf("Received:%s\n",recv);
		tcflush(fd,TCIFLUSH);	//clear data
	}	
	if(close(fd)==-1){
		perror("close ttyS0 fail!");
		exit(1);	
	}
	printf("exit successful!\n");
	exit(1);
}
