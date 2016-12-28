#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
//定义缓冲区宏大小
#define BUFFER_LENGTH 1024

int main(int argc, char *argv[]) {
	//声明地址结构变量，连接描述符，地址大小，缓冲区
   	struct sockaddr_in server_addr;
	int sockfd;
    	socklen_t server_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n;

	//判断参数输入完整性
    	if (argc != 3){
		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
		exit(1);
	}

	//创建客服端socket
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
       if (sockfd == -1){
		perror("Client created udp socket fail!\n");
		exit(1);
	}

	//填充服务器地址及端口号，并转化为网络字节序
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], &server_addr.sin_addr) == 0){
		perror("Client inet_aton fail!");
		exit(1);
	}

	//while循环，发送接收消息
	while(1){
	//缓冲区清0
	memset(buf, 0, BUFFER_LENGTH);
	printf("Send:");
	gets(buf);
	//发送
    	n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (n == -1){
		perror("Client fail to send");
		exit(1);
	}
	//客服端结束判断
	if(strcmp(buf,"byebye") == 0){
		break;
	}
	//缓冲区清0
       memset(buf, 0, BUFFER_LENGTH);
	//接受
    	n = recvfrom(sockfd, buf, BUFFER_LENGTH, 0, (struct sockaddr *)&server_addr, &server_addr_len);
    	if (n == -1){
    		perror("Client fail to receive");
    		exit(1);
    	}
    	else{
    		buf[n] = '\0';
    		printf("Server: %s",buf);
		printf("(Server_IP:%s,Server_Port:%d)\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
		//结束判断
		if(strcmp(buf,"byebye") == 0){
			printf("UDP Server is closed!\n");
			break;
		}
    	}
	}
	//客服端socket关闭
       if (close(sockfd) == -1){
		perror("fail to close");
		exit(1);
	}
	puts("UDP Client is closed!\n");
	return EXIT_SUCCESS;
}
