#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

//���ÿͷ��˻������������С
#define BUFFER_LENGTH 1024


int main(int argc,char*argv[]){
	//�����������˵�ַ�ṹ�壬���������������������������˿ں�
	struct sockaddr_in server_addr;
	int connfd;
	char buf[BUFFER_LENGTH];
	int n;
	//unsigned short portnum=argv[2];
	
	//�������ӷ�������socket
	connfd=socket(AF_INET,SOCK_STREAM,0);
	if(connfd==-1){
		perror("client socket fail!\n");	
	}
	printf("client socket ok!\n");

	//���������˵�ַ�ṹ
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);    //�ػ�
	server_addr.sin_port=htons(atoi(argv[2]));			//�󶨶˿�

       //�ͷ�����������
	if(connect(connfd,(struct sockaddr *)(&server_addr), sizeof(struct sockaddr))<0){
		perror("connect fail!");
		exit(1);	
	}
	printf("connect ok!\n");
 
       //whileѭ��
	while(1){
	//��ջ�����
	memset(buf,0,BUFFER_LENGTH);
	printf("Send:");
	gets(buf);
	//��������˷�����Ϣ
	n=send(connfd,buf,strlen(buf),0);
	if(n==-1){
		perror("send fail!");
		exit(1);	
	}
	//�����ж�
	if(strcmp(buf,"byebye")==0){
		break;	
	}
	//�������Է������˵���Ϣ
	n=recv(connfd,buf,BUFFER_LENGTH,0);
	if(n==-1){
		perror("recv fail!");
		exit(1);
	}
	//��ӡ��Ϣ
	else{
		buf[n]='\0';
		printf("Server:%s\n",buf);	
	}
	//�����ж�
	if(strcmp(buf,"byebye")==0){
		break;	
	}

	}
	//�ر�����������
	if(close(connfd==-1)){
		perror("close fail!");
		exit(1);	
	}
	puts("TCP Server is closed!\n");
	return EXIT_SUCCESS;
}
