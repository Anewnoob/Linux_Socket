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
	//声明地址结构变量，连接描述符，地址大小，缓冲区大小
	struct sockaddr_in server_addr, client_addr;
	int sockfd;
	socklen_t client_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n;
	
	//判断输入参数是否完整
	if (argc != 3){
		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
		exit(1);
	}

	//创建描述符
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1){
		perror("created udp socket fail!");
		exit(1);
	}
	
	//填充服务器地址
       bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], &server_addr.sin_addr) == 0){
		perror("UDP inet_aton fail!");
		exit(1);
	}
	
	//绑定地址
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		perror("UDP bind fail!");
		exit(1);
	}

	//while循环，接受发送消息
       while(1){
	//缓冲区清0
    	memset(buf, 0, BUFFER_LENGTH);
	//接受
    	n = recvfrom(sockfd, buf, BUFFER_LENGTH, 0, (struct sockaddr *)&client_addr, &client_addr_len);
	if (n == -1){
		perror("fail to receive");
		exit(1);
	}
    	else{
    		buf[n] = '\0';
		//打印消息和IP,端口号
		printf("Client:%s",buf);
		printf("(Client_IP:%s,Client_Port:%d)\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		//客服端结束判断
		if(strcmp(buf,"byebye") == 0){
			printf("%s quit\n",inet_ntoa(client_addr.sin_addr));
			continue;
		}
		//清0
		//memset(buf, 0, BUFFER_LENGTH);
		sprintf(buf,"%s(Client_IP:%s,Client_Port:%d)\n",buf,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		//printf("Send:");
    		//gets(buf);
    		n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    		if (n == -1){
			perror("fail to reply");
			exit(1);
		}
		//服务器端结束判断
		if(strcmp(buf,"byebye") == 0){
			break;
		}
   	}
	}
	//关闭服务器端Socket
          if (close(sockfd) == -1){
		perror("UDP server fail to closed");
		exit(1);
	}
	puts("UDP Server is closed!\n");
	return EXIT_SUCCESS;
}
