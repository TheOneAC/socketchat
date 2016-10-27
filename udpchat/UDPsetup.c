#include "UDPsetup.h"

const int server_port = 6789;
const char *server = "127.0.0.1";
const char *LOGIN = "@LOGIN";
const char *QUIT = "@QUIT";

void err_ne(const char* err_msg){
	perror(err_msg);
	exit(1);
}
int set_sock(){
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
		err_ne("init socket failure");
	return sock;
}
void set_serveraddr(sockaddr_in *serveraddr ){
	memset(serveraddr,0,sizeof(*serveraddr));
	serveraddr->sin_family = AF_INET;
	serveraddr->sin_port = htons(server_port);
	serveraddr->sin_addr.s_addr = htonl(INADDR_ANY);
}
void client_set_server(sockaddr_in *servaddr){
	memset(servaddr,0,sizeof(*servaddr));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(server_port);
	inet_pton(AF_INET, server, &(servaddr->sin_addr));
}


void catstr(char *str, const char* stri, const char* strn){
	strcat(str, stri);
	strcat(str, " ");
	strcat(str, strn);
}
void send_login(const char *username,int client_sock, sockaddr* servaddr, socklen_t servlen){
	char msg_send[MAXLINE];
	memset(msg_send, 0, sizeof(msg_send));
	catstr(msg_send, LOGIN, username);
	int status = sendto(client_sock, msg_send,sizeof(msg_send), 0, servaddr, servlen);
	if(status < 0)
		err_ne("sengdlogin failure");
}



void set_Client(Client *this, sockaddr clientaddr, const char *name){
	memcpy( &(this->client_addr), &clientaddr, sizeof(clientaddr) );
	memset(this->username,0, MAXLINE);
	strcpy(this->username, name);
	this->next = NULL;
	return;
}
void head_insert(Client *this, Client *head){
	this->next = head->next;
	head->next = this;
	return;
}
Client *find_client_byname(Client *head, const char *name){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(strcmp(name, cur->username) == 0)
			return cur;
	}
	return NULL;
}
Client *find_client(Client *head, sockaddr client_addr){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(memcmp(&client_addr, &(cur->client_addr),sizeof(client_addr)) == 0)
			return cur;
	}
	return NULL;
}
void delete_client(Client *head, sockaddr client_addr){
	Client *cur = head;
	for(;cur->next != NULL; cur = cur->next){
		Client *curnext = cur->next;
		if(memcmp(&client_addr, &(curnext->client_addr),sizeof(client_addr)) == 0){
			cur->next = curnext->next;
			free(curnext);
			return;
		}
	}
	return;
}
void deal_msg(const char *buffer, char *name, char *msg){
	size_t bufferlen = strlen(buffer);
	int index = -1, i = 0;
	for(;i< bufferlen; i++){
		if(buffer[i] == ' '){
			index = i;
			break;
		}
	}
	if(index != -1){
		strncpy(name, buffer+1, index-1);
		strncpy(msg, buffer+index+1,bufferlen-index);
		size_t msglen = strlen(msg);
		if(msg[msglen-1] != '\n');{
			//msg[msglen] = '\n';
			msg[msglen+1] = 0;
		}
	}else 
		strncpy(name, buffer+1, strlen(buffer)-1);
	return ;
}