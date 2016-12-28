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


int main(int argc,char* argv[]){
	//�����������ˣ��ͷ��˵�ַ�ṹ�壬������������������������
	struct sockaddr_in server_addr,client_addr;
	int listenfd,connfd;
	int conn[10];
	socklen_t client_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n,maxfd,i,j=0;
	fd_set allset,rset,wset,eset;
	//struct timeval* timeout;

	//�жϲ����Ƿ���������
	if(argc != 3){
		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
		exit(1);
	}

	//���������׽���
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1){
		perror("creat listenfd fail!");
		exit(1);
	}

	//����������ַ
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	if(inet_aton(argv[1],&server_addr.sin_addr) == 0){
		perror("inet_aton fail!");
		exit(1);
	}

	//�󶨷�������ַbind
	if(bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
		perror("server bind fail!");
		close(listenfd);
		exit(1);
	}
	
	//���ü�����
	if(listen(listenfd,LISTENQ) < 0 ){
		perror("server listen fail!");
		exit(1);
	}
	//��ʼ��������������
	FD_ZERO(&allset);
	
	//�Ѽ���������������������
	FD_SET(listenfd,&allset);
	FD_SET(0,&allset);
	maxfd=listenfd;

	for(i=0;i<10;i++){
		conn[i]=-1;	
	}

	do{
		rset=allset;
		select(maxfd+1,&rset,NULL,NULL,NULL);
		
		if(FD_ISSET(listenfd,&rset) == 1){
			//��������
			connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
			if(connfd == -1){
				perror("server accept fail!");
				exit(1);
			}
			//��ȡ��maxfdֵ
			maxfd=connfd>maxfd?connfd:listenfd;
			//���½��յ���������������������������
			FD_SET(connfd,&allset);
			//�ҵ���ǰ��û�õ������λ     
			for(j=0,j<10,j++){
			if(conn[j]==-1){break;}
			
                   }
			conn[j]=connfd;
		}
		if(FD_ISSET(0,&rset)==1){
		  memset(buf,0,BUFFER_LENGTH);
		  read(0, buf,BUFFER_LENGTH);
		  if(strcmp(buf,"exit\n")==0){
		         break;
		  }
		}
		for(i=0;i<10;i++){
		if(FD_ISSET(conn[i],&rset) == 1){
			//recv������Ϣ
			memset(buf,0,BUFFER_LENGTH);
			n=recv(conn[i],buf,BUFFER_LENGTH,0);
			if(n == -1){
				perror("server recv fail��");
				exit(1);
			}
			else{
				buf[n]='\0';
				printf("Client:%s\n",buf);
				n=send(conn[i],buf,strlen(buf),0);
				if(n == -1){
					perror("server send fail!");
					exit(1);
				}
				if(strcmp(buf,"byebye") == 0){
					printf("Client disconnect with server!\n");
					conn[i]=-1;
					close(conn[i]);
					FD_CLR(conn[i],&rset);
					FD_CLR(conn[i],&allset);
					continue;
				}
			}
		}}
	}while(1);
	//�رշ���������������
	if(close(listenfd) == -1){
		perror("listenfd closed fail!");
		exit(1);
	}
	puts("Server is closed!");
	return EXIT_SUCCESS;
}
