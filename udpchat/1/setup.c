#include "setup.h"

const int server_port = 6789;
const size_t MAXLINE = 1024;
const int MAXCLIENTNUMBER = 256;
const char *server = "127.0.0.1";
const char *HELLO = ">HELLO";
const char *QUIT = ">QUIT";


/************systerm error**********/ 
void err(const char *msg){
	perror(msg);
	exit(1);
}

void catstr(char *str, const char *strc, const char *strn){
	strcat(str, strc);
	strcat(str, " ");
	strcat(str, strn);
}
/************input with no clientname error**********/
void inputErr(const char* msg, const char* detail){
	fputs(msg,stderr);
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
/************tell server client login**********/
void sendinfo(const char* username, int sock, const sockaddr *servaddr, socklen_t servlen){
	char message_send[MAXLINE];
	memset(message_send,0,sizeof(message_send));
	catstr(message_send,HELLO,username);
	int status;
	status = sendto(sock, message_send, strlen(message_send), 0, servaddr, servlen);
	if(status < 0)
		err("send message failure");
}

/************ init clientnode**********/
void setClient(Client *cur, const sockaddr *addrptr, const char *name){
	memcpy(&(cur->clientaddr),addrptr,sizeof(*addrptr));
	memset(cur->username, 0, sizeof(cur->username));
	strcpy(cur->username, name);
	cur->next = NULL;
	return;
}
/************ head insert clientnode**********/
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
Client* findClientbyname(Client* head, const char* username){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(strcmp(username, cur->username)== 0)
			return cur;
	}
	return NULL;
}
/************ deal with clientname**********/
void dealname(const char *buffer, char *name, char *msg){
	size_t bufferlen = strlen(buffer);
	int index= -1, i=0;
	for(;i< bufferlen;i++){
		if(buffer[i] == ' '){
			index = i;
			break;
		}
	}
	if(index == -1){
		strncpy(name, buffer+1, index -1);
		printf("name = %s : ", name);
		strncpy(msg, buffer+index+1, bufferlen-index);
		size_t msglen =strlen(msg);
		if(msg[msglen-1] != '\n'){
			msg[msglen] = '\n';
			msg[msglen+1] = 0;
		}
		printf("msg = %s : ", msg);
	}else
		strncpy(name, buffer+1, strlen(buffer)-1);
	return;
}
