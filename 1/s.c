#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

//���建�������˿ںţ������������
#define BUFFER_LENGTH 1024
#define SERV_PORT 13000
#define LISTENQ 10

int main(){
	//�����������ˣ��ͷ��˵�ַ�ṹ�壬������������������������
	struct sockaddr_in server_addr,client_addr;
	int listenfd,connfd;
	socklen_t client_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n;
	
	//��������socket
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd==-1){
		perror("listenfd fail!");
		exit(1);
	}
	printf("socket ok!\n");

	//���������˵�ַ�ṹ
	bzero(&server_addr,sizeof(server_addr)); 
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(SERV_PORT);
	
	//�󶨷�������ַ��socket
	if(bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
		perror("bind fail!");
		exit(1);
	}
	printf("bind ok!\n");

	//���ü�����
	if(listen(listenfd,LISTENQ)<0){
		perror("listen fail!");			
	}
	printf("listen ok!\n");

	//whilieѭ��
	while(1){
		//�����ȴ��ͷ�����������
		connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
		if(connfd<0){
			perror("accept failed");
			exit(1);
		}
		printf("accept ok!\n");
		while(1){
		memset(buf,0,BUFFER_LENGTH);       //��ջ�����
		//���ܿͷ��˷�����message
		n=recv(connfd,buf,BUFFER_LENGTH,0);
		if(n==-1){
			perror("recv fail!");
			exit(1);					
		}
		else{
			buf[n]='\0';               //���ܵ��Ŀͷ��˵��ַ�����ĩβ���'\0'
			printf("client:%s\n",buf);
			//�����ж�
			if(strcmp(buf,"byebye")==0){
				close(connfd);
				break;			
			}
			printf("Send:");
			gets(buf);
			//��ͷ��˷���message
			n=send(connfd,buf,strlen(buf),0);
			if(n==-1){
				perror("send fail!");
				exit(1);			
			}
			//�����ж�
			if(strcmp(buf,"byebye")==0){
				close(connfd);
				break;		
			}		
		   }
		}
		printf("client disconnect!\n");
	}
	//�رշ���������������
	if(close(listenfd)==-1){
		perror("close fail!");
		exit(1);	
	}
	puts("TCP Server is closed!");
	return EXIT_SUCCESS;	
}

