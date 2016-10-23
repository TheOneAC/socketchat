#include "setup.h"

extern const int PORT; 
extern const char *SERVERAADR;
extern const int MAXLINE; 
#define CONNECT_LIMIT 10

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
	int message_len,i, broadcast;
	while( ( message_len = recv(servering_clientfd, message_receive, sizeof(message_receive),0) ) > 0 ){
		/*******get target id to predcate  whether it's broaddcast or not*****/
		char target_cid = message_receive[message_len-1];
		broadcast = 0;
		if(target_cid =='a')broadcast =1;
		/*******info for server to show the chat*****/
		for(i =0 ;i< CONNECT_LIMIT; i++){
			if(socklist[i].client_fd == servering_clientfd)
				printf("message from %s.%d ",inet_ntoa(socklist[i].clientaddr.sin_addr),ntohs(cur_sock->clientaddr.sin_port));
			
		}
		printf("%s ",message_receive );
		if(broadcast)printf("message sendto all the client\n");
		for(i =0 ;i< CONNECT_LIMIT; i++){
			if(socklist[i].cid == target_cid)
				printf("message sendto %s.%d \n",inet_ntoa(socklist[i].clientaddr.sin_addr),ntohs(cur_sock->clientaddr.sin_port));
			
		}
		/*******roll quest to find the target pclient_fd to transmit*****/
		for(i =0 ;i< CONNECT_LIMIT; i++){
			if(i == cur_sock->num)continue;
			else if(broadcast || target_cid == socklist[i].cid){
				status = send(socklist[i].client_fd, message_receive, message_len, 0);
				errn(status,"send failure");
			}
		}
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
	int status;
	int server_fd =SetTCPSock();
	struct sockaddr_in server;
	SetTCPServer(&server);
	status = bind(server_fd, (struct sockaddr *)&server,sizeof(server)); 
	errn(status,"bind failure");
	listen(server_fd,CONNECT_LIMIT);

	int client_len = sizeof(struct sockaddr_in);
	int socknum =0 ;
	while((socklist[socknum].client_fd = accept(server_fd,(struct sockaddr *)&socklist[socknum].clientaddr,(socklen_t*)&client_len)) > 0){
		/************client info (one char for id)****************/
		printf("------A new connection from:");
		char client_info[MAXLINE];
		status = recv(socklist[socknum].client_fd, client_info, sizeof(client_info),0);
		errn(status,"receice client_info failure");
		socklist[socknum].cid = client_info[0];
		printf("client[%c] is online now------\n",client_info[0]);
		socklist[socknum].num = socknum;
		status = pthread_create(&socklist[socknum].thread,NULL, &server_thread,(void*)&socklist[socknum]);
		if( status < 0){
			perror("create thread failure");
			return -1;
		}
		else socknum++;
	}
	return 0;
}
