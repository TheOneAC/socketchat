#include "setup.h"

const int server_port = 5678;
const size_t MAXLINE = 1024;
const int MAXCLIENTNUMBER = 64;
const char *server = "127.0.0.1";
const char *HELLO = "@HELLO";
const char *END = "@END";



void err(const char *msg){
	perror(msg);
	exit(1);
}
void catstr(char *str, const char *strc, const char *strn){
	strcat(str, strc);
	strcat(str, " ");
	strcat(str, strn);
}
void inputErr(const char* mes, const char* detail){
	fputs(mes,stderr);
	fputs(": ",stderr);
	fputs(detail, stderr);
	fputs("\n",stderr);
	exit(1);
}
void setupUDPServer(sockaddr_in* serverAddr){
	serverAddr->sin_family = AF_INET;
	serverAddr->sin_port = htons(server_port);
	serverAddr->sin_addr.s_addr = htonl(INADDR_ANY);
}
void setupUDPClient(sockaddr_in* clientAddr){
	clientAddr->sin_family = AF_INET;
	clientAddr->sin_port = htons(server_port);
	inet_pton(AF_INET, server, &(clientAddr->sin_addr));
}
int setUDPSocket(){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
		err("socket init failure");
	return sock;
}
void sendinfo(const char* username, int sock, const sockaddr *servaddr, socklen_t servlen){
	char message_send[MAXLINE];
	memset(message_send,0,sizeof(message_send));
	catstr(message_send,HELLO,username);
	int status;
	status = sendto(sock, message_send, strlen(message_send), 0, servaddr, servlen);
	if(status < 0)
		err("send message failure");
}
void setClient(Client *cur, const sockaddr *addrptr, const char *name){
	memcpy(&(cur->clientaddr),addrptr,sizeof(*addrptr));
	memset(cur->username, 0, sizeof(cur->username));
	strcpy(cur->username, name);
	cur->next = NULL;
	return;
}
void headInsert(Client *cur, Client* head){
	cur->next = head->next;
	head->next = cur;
	return;
}
Client* findClient(Client* head, const sockaddr *addrptr){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(memcmp(addrptr,&(cur->clientaddr),sizeof(*addrptr))== 0)
			return cur;
	}
	return NULL;
}
void deleteClient(Client * head, const sockaddr* addrptr)
{
	Client *cur = head->next;
	for(; cur->next != NULL; cur= cur->next){
		Client* curnext = cur->next;
		if(memcmp(addrptr,&(cur->clientaddr),sizeof(*addrptr))== 0){
			cur->next = curnext->next;
			free(curnext);
			return;
		}
	}
}
Client* findClientbyname(Client* head, const char* name){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(strcmp(name, cur->username)== 0)
			return cur;
	}
	return NULL;
}
void dealname( char *buffer, char *name, char *msg){
	size_t bufferlen = strlen(buffer), index= -1, i=0;
	for(;i< bufferlen;i++){
		if(buffer[i] == ' '){
			index = i;
			break;
		}
	}
	if(index == -1){
		strncpy(name, buffer+1, index -1);
		strncpy(msg, buffer+index+1, bufferlen-index);
		size_t msglen =strlen(msg);
		if(msg[msglen-1] != '\n'){
			msg[msglen] = '\n';
			msg[msglen+1] = 0;
		}
	}else
		strncpy(name, buffer+1, strlen(buffer)-1);
	return;
}
