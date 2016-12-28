#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
//���建�������С
#define BUFFER_LENGTH 1024

int main(int argc, char *argv[]) {
	//������ַ�ṹ��������������������ַ��С��������
   	struct sockaddr_in server_addr;
	int sockfd;
    	socklen_t server_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n;

	//�жϲ�������������
    	if (argc != 3){
		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
		exit(1);
	}

	//�����ͷ���socket
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
       if (sockfd == -1){
		perror("Client created udp socket fail!\n");
		exit(1);
	}

	//����������ַ���˿ںţ���ת��Ϊ�����ֽ���
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], &server_addr.sin_addr) == 0){
		perror("Client inet_aton fail!");
		exit(1);
	}

	//whileѭ�������ͽ�����Ϣ
	while(1){
	//��������0
	memset(buf, 0, BUFFER_LENGTH);
	printf("Send:");
	gets(buf);
	//����
    	n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (n == -1){
		perror("Client fail to send");
		exit(1);
	}
	//�ͷ��˽����ж�
	if(strcmp(buf,"byebye") == 0){
		break;
	}
	//��������0
       memset(buf, 0, BUFFER_LENGTH);
	//����
    	n = recvfrom(sockfd, buf, BUFFER_LENGTH, 0, (struct sockaddr *)&server_addr, &server_addr_len);
    	if (n == -1){
    		perror("Client fail to receive");
    		exit(1);
    	}
    	else{
    		buf[n] = '\0';
    		printf("Server: %s",buf);
		printf("(Server_IP:%s,Server_Port:%d)\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
		//�����ж�
		if(strcmp(buf,"byebye") == 0){
			printf("UDP Server is closed!\n");
			break;
		}
    	}
	}
	//�ͷ���socket�ر�
       if (close(sockfd) == -1){
		perror("fail to close");
		exit(1);
	}
	puts("UDP Client is closed!\n");
	return EXIT_SUCCESS;
}
