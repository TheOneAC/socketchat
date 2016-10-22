#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 6789
#define SERVERADDR "127.0.0.1"
#define MAXLINE 100

int main(int argc, char const *argv[])
{
	int client_fd;
	struct sockaddr_in server;
	client_fd = socket(AF_INET, SOCK_STREAM,0);
	if(client_fd == -1){
		perror("fail to init client_fd socket");
		return -1;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(SERVERADDR);
	if((connect(client_fd,(struct sockaddr*)&server,sizeof(server))) < 0){
		perror("fail to init connect to server");
		return -1;
	}
	puts("connection server success!");
	char message_send[MAXLINE],message_receive[MAXLINE];
	char id = 'A';
	while(1){
		/*******input*****/
		printf("$$ > :");
		memset(message_send,0,sizeof(message_send));
		scanf("%s",message_send);
		int len = strlen(message_send);
		//fgets(message_send,sizeof(message_send),stdin);

		/*******test for quit and empty*****/
		if(len == 0){
			puts("Empty message!");
			continue;
		}
		else if(len == 1 && message_send[0] == 'q')break;

		/*******ADD source ID and target ID*****/
		//message_send[len++]= id;//source id
		//message_send[len++]= id + '2';//target id

		int message_len = send(client_fd,message_send,len,0);
		if(message_len < 0){
			perror("fail to send message");
			return -1;
		} 
		memset(message_receive,0,sizeof(message_receive));
		message_len = recv(client_fd,message_receive,sizeof(message_receive),0);
		if(message_len < 0){
			perror("fail to receive message");
			return -1;
		}
		/*******deal with ID*****/
		//char from_id = message_receive[strlen(message_receive)-1];
		message_receive[strlen(message_receive)] = '\0';
		printf("** < [] : %s\n", message_receive);
	}
	return 0;
}