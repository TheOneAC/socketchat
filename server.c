#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 6789
#define SERVERAADR "127.0.0.1"
#define MAXLINE 100
#define CONNECT_LIMIT  10

typedef struct 
{
	char cid;
	int num;
	pthread_t thread;
	int client_fd;
	struct sockaddr_in clientaddr;
}sockid;
sockid socklist[CONNECT_LIMIT]; 

void* server_thread(void* arg){
	int status;
	sockid *cur_sock = ((sockid *)arg);
	int servering_clientfd = cur_sock->client_fd;
	char message_receive[MAXLINE];

	/**test for access global variaty***
	int i ;
	for(i =0 ;i< CONNECT_LIMIT; i++){
		if(i == cur_sock->num){
			printf("正在与您聊天的客户端是：%s: %d\n",inet_ntoa(socklist[i].clientaddr.sin_addr),ntohs(cur_sock->clientaddr.sin_port));
		}
	}
	*/
	int message_len;
	while( ( message_len = recv(servering_clientfd, message_receive, sizeof(message_receive),0) ) > 0 ){
		cur_sock->cid = message_receive[message_len -2];
		char target_cid = message_receive[--message_len];
		message_receive[message_len] = '\0';

		int i, broadcast = 0;
		if(target_cid =='a')broadcast =1;
		for(i =0 ;i< CONNECT_LIMIT; i++){
			if(i == cur_sock->num)continue;
			else if(broadcast || target_cid == socklist[i].cid){
				status = send(socklist[i].client_fd, message_receive, message_len, 0);
				//printf("正在与您聊天的客户端是：%s: %d\n",inet_ntoa(socklist[i].clientaddr.sin_addr),ntohs(cur_sock->clientaddr.sin_port));
			}
		}
		puts(message_receive);
		//send(servering_clientfd, message_receive, message_len, 0);
	}
	if(message_len == 0){
		puts("client not connect");
		fflush(stdout);
	}
	else if(message_len == -1){
		perror("\n recv failed");
	}
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in server, client[CONNECT_LIMIT];
	server_fd =socket(AF_INET,SOCK_STREAM,0);
	if(server_fd <0){
		perror("server socket init failure");
		return -1;
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	if(bind(server_fd, (struct sockaddr *)&server,sizeof(server)) < 0){
		perror("bind failure");
		return -1;
	}
	listen(server_fd,3);

	int client_len = sizeof(struct sockaddr_in);
	int socknum =0 ;
	while((socklist[socknum].client_fd = accept(server_fd,(struct sockaddr *)&socklist[socknum].clientaddr,(socklen_t*)&client_len)) > 0){
		printf("------A new connection -----\n");
		//pthread_t tmp_thread ;
		//socklist[socknum].client_fd = client_fd;
		socklist[socknum].num = socknum;
		if(pthread_create(&socklist[socknum].thread,NULL, &server_thread,(void*)&socklist[socknum]) < 0){
			perror("create thread failure");
			return -1;
		}
		else {
			//socklist[socknum].thread =  tmp_thread;
			socknum++;
		}
	}
	
	return 0;
}
