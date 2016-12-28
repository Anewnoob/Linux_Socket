#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

//设置客服端缓冲区宏参数大小
#define BUFFER_LENGTH 1024


int main(int argc,char*argv[]){
	//声明服务器端地址结构体，连接描述符，缓冲区，服务器端口号
	struct sockaddr_in server_addr;
	int connfd;
	char buf[BUFFER_LENGTH];
	int n;
	//unsigned short portnum=argv[2];
	
	//创建连接服务器的socket
	connfd=socket(AF_INET,SOCK_STREAM,0);
	if(connfd==-1){
		perror("client socket fail!\n");	
	}
	printf("client socket ok!\n");

	//填充服务器端地址结构
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);    //回环
	server_addr.sin_port=htons(atoi(argv[2]));			//绑定端口

       //客服端请求连接
	if(connect(connfd,(struct sockaddr *)(&server_addr), sizeof(struct sockaddr))<0){
		perror("connect fail!");
		exit(1);	
	}
	printf("connect ok!\n");
 
       //while循环
	while(1){
	//清空缓冲区
	memset(buf,0,BUFFER_LENGTH);
	printf("Send:");
	gets(buf);
	//向服务器端发送消息
	n=send(connfd,buf,strlen(buf),0);
	if(n==-1){
		perror("send fail!");
		exit(1);	
	}
	//结束判断
	if(strcmp(buf,"byebye")==0){
		break;	
	}
	//接受来自服务器端的消息
	n=recv(connfd,buf,BUFFER_LENGTH,0);
	if(n==-1){
		perror("recv fail!");
		exit(1);
	}
	//打印消息
	else{
		buf[n]='\0';
		printf("Server:%s\n",buf);	
	}
	//结束判断
	if(strcmp(buf,"byebye")==0){
		break;	
	}

	}
	//关闭连接描述符
	if(close(connfd==-1)){
		perror("close fail!");
		exit(1);	
	}
	puts("TCP Server is closed!\n");
	return EXIT_SUCCESS;
}
