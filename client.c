#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define PORT 6789
#define SERVERADDR "127.0.0.1"
#define MAXLINE 100
#define CONNECT_LIMIT  10
#define SOURCE_ID 'B'
#define TARGET_ID 'C'
/* change SOURCE_ID id and TARGET_ID for every new client [A~Z]
 * if TARGET_ID is 'a' which means 'all', then it will breadcast to all the client
 */
/*********thread for receive********/
void* client_receive(void*arg){
	int status;
	int client_fd = *(int *)arg;
	char message_receive[MAXLINE];
	int message_len;

	while(1)
	{
		memset(message_receive,0,sizeof(message_receive));
		message_len = recv(client_fd,message_receive,sizeof(message_receive),0);
		if(message_len < 0){
			perror("fail to receive message");
			break;
		}
		/*******deal with source ID*****/
		char from_cid = message_receive[strlen(message_receive)-1];
		message_receive[strlen(message_receive)-1] = '\0';
		
		printf("\nmeesage from [%c]client : %s\n$$ > :",from_cid, message_receive);
		fflush(stdout);
	}
	exit(0);
}
/*********thread for send********/
void* client_send(void* arg){
	int client_fd = (*(int *)arg);
	char message_send[MAXLINE];
	int status;
	while(1)
	{
		memset(message_send,0,sizeof(message_send));
		fgets(message_send,sizeof(message_send),stdin);
		int len = strlen(message_send)-1;// exception for '\n' in the last
		/*******test for quit and empty*****/
		if(len == 0){
			puts("Empty message!");
			continue;
		}
		else if(len == 1 && message_send[0] == 'q')break;
		/*******ADD target ID and send*****/
		message_send[len++]= TARGET_ID;//target id
		if(send(client_fd,message_send,len,0) < 0){
			perror("fail to send message");
			break;
		} 
		printf("$$ > :");
	}
	exit(0);
}

int main(int argc, char const *argv[])
{
	int client_fd;
	struct sockaddr_in server;
	/*******init client_fd and server address*****/
	client_fd = socket(AF_INET, SOCK_STREAM,0);
	if(client_fd == -1){
		perror("fail to init client_fd socket");
		return -1;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(SERVERADDR);
	/*******connect to server *****/
	if((connect(client_fd,(struct sockaddr*)&server,sizeof(server))) < 0){
		perror("fail to init connect to server");
		return -1;
	}
	puts("login success!");
	char client_info[MAXLINE];
	client_info[0] = SOURCE_ID;
	if( send(client_fd,client_info,MAXLINE,0) < 0){
		perror("send client_info faliure");
		return -1;
	}
	printf("$$ > :");

	/*******threads for read and write *****/
	pthread_t rthread, wthread;
	int status;
	status = pthread_create(&wthread,NULL, &client_send,(void*)&client_fd);
	if(status < 0){
		perror("create rthread failure");
		return -1;
	}
	status = pthread_create(&rthread,NULL, &client_receive,(void*)&client_fd);
	if(status < 0){
		perror("create wthread failure");
		return -1;
	}
	status = pthread_join(wthread,NULL);
	if(status != 0){
		perror("join wthread failure");
		return -1;
	}
	status = pthread_join(rthread,NULL);
	if(status != 0){
		perror("join rthread failure");
		return -1;
	}
	close(client_fd);
	return 0;
}