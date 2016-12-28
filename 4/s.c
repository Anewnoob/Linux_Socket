#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>

//定义缓冲区，端口号，监听数宏参数
#define BUFFER_LENGTH 1024
#define SERV_PORT 13000
#define LISTENQ 10


int main(int argc,char* argv[]){
	//声明服务器端，客服端地址结构体，监听，连接描述符，缓冲区
	struct sockaddr_in server_addr,client_addr;
	int listenfd,connfd,i=0,j=0;
	socklen_t client_addr_len = sizeof(struct sockaddr);
	char buf[BUFFER_LENGTH];
	int n,maxfd;
	fd_set allset,rset;
	pid_t pid[10];
	pid_t c_pid;
	int conn[10];
	
	//判断参数是否输入完整
	if(argc != 3){
		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
		exit(1);
	}

	//创建监听套接字
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1){
		perror("creat listenfd fail!");
		exit(1);
	}

	//填充服务器地址
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(atoi(argv[2]));
	if(inet_aton(argv[1],&server_addr.sin_addr) == 0){
		perror("inet_aton fail!");
		exit(1);
	}

	//绑定服务器地址bind
	if(bind(listenfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
		perror("server bind fail!");
		close(listenfd);
		exit(1);
	}
	
	//设置监听数
	if(listen(listenfd,LISTENQ) < 0 ){
		perror("server listen fail!");
		exit(1);
	}

	//初始化pid值
	for(i=0;i<10;i++){
		pid[i]=-1;
	}
	
	//初始化conn值
	for(i=0;i<10;i++){
		conn[i]=-1;
	}

	//初始化描述符集清零
	FD_ZERO(&allset);
	
	//把监听描述符放入描述符集
	FD_SET(listenfd,&allset);
	FD_SET(0,&allset);
	maxfd=listenfd;

	do{
		rset=allset;
		select(maxfd+1,&rset,NULL,NULL,NULL);
	
		if(FD_ISSET(listenfd,&rset) == 1){
			//阻塞接受
			connfd=accept(listenfd,(struct sockaddr*)&client_addr,&client_addr_len);
			if(connfd == -1){
				perror("server accept fail!");
				exit(1);
			}
			//找到数组中最前面未用的位
			for(i=0;i<10;i++){
				if(pid[i]==-1){j=i;break;}
			}
			pid[j]=fork();
			conn[j]=connfd;

			if(pid[j] == 0){   //in child
				//close listenfd
				close(listenfd);
				while(1){
					memset(buf,0,BUFFER_LENGTH);
					n=recv(conn[j],buf,BUFFER_LENGTH,0);
					if(n == -1){
						perror("server recv fail！");
						exit(1);
					}
					else{
						buf[n]='\0';
						printf("Client:%s\n",buf);
						n=send(conn[j],buf,strlen(buf),0);
						if(n == -1){
							perror("server send fail!");
							exit(1);
						}
						if(strcmp(buf,"byebye") == 0){
							printf("Client disconnect with server!\n");
							close(conn[j]);
							//回收不用的数组
							pid[j]=-1;
							conn[j]=-1;
							exit(EXIT_SUCCESS);
						}
					}
				}
			}
			//in parent
			else{
				close(conn[j]);
		       		continue;
			}
		}
		//服务器退出监听
		if(FD_ISSET(0,&rset)==1){
		  memset(buf,0,BUFFER_LENGTH);
		  read(0, buf,BUFFER_LENGTH);
		  if(strcmp(buf,"exit\n")==0){
			
		         break;
		  }
		}
	}while(1);
	//关闭服务器监听描述符
	if(close(listenfd) == -1){
		perror("listenfd closed fail!");
		exit(1);
	}
	puts("Server is closed!");
	return EXIT_SUCCESS;
}
