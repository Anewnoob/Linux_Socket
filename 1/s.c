#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

//定义缓冲区，端口号，监听数宏参数
#define BUFFER_LENGTH 1024
#define SERV_PORT 13000
#define LISTENQ 10

int main(){
	//声明服务器端，客服端地址结构体，监听，连接描述符，缓冲区
	struct sockaddr_in server_addr,client_addr;
	int listenfd,connfd;
	socklen_t client_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n;
	
	//创建监听socket
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
		perror("listenfd fail!");
		exit(1);
	}
	printf("socket ok!\n");

	//填充服务器端地址结构
	bzero(&server_addr,sizeof(server_addr)); 
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(SERV_PORT);
	
	//绑定服务器地址与socket
	if(bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
		perror("bind fail!");
		exit(1);
	}
	printf("bind ok!\n");

	//设置监听数
	if(listen(listenfd,LISTENQ)<0){
		perror("listen fail!");			
	}
	printf("listen ok!\n");

	//whilie循环
	while(1){
		//阻塞等待客服端申请连接
		connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
		if(connfd<0){
			perror("accept failed");
			exit(1);
		}
		printf("accept ok!\n");
		while(1){
		memset(buf,0,BUFFER_LENGTH);       //清空缓冲区
		//接受客服端发来的message
		n=recv(connfd,buf,BUFFER_LENGTH,0);
		if(n==-1){
			perror("recv fail!");
			exit(1);					
		}
		else{
			buf[n]='\0';               //接受到的客服端的字符串后末尾添加'\0'
			printf("client:%s\n",buf);
			//结束判断
			if(strcmp(buf,"byebye")==0){
				close(connfd);
				break;			
			}
			printf("Send:");
			gets(buf);
			//向客服端发送message
			n=send(connfd,buf,strlen(buf),0);
			if(n==-1){
				perror("send fail!");
				exit(1);			
			}
			//结束判断
			if(strcmp(buf,"byebye")==0){
				close(connfd);
				break;		
			}		
		   }
		}
		printf("client disconnect!\n");
	}
	//关闭服务器监听描述符
	if(close(listenfd)==-1){
		perror("close fail!");
		exit(1);	
	}
	puts("TCP Server is closed!");
	return EXIT_SUCCESS;	
}

