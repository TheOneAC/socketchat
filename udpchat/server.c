/******UDPserver*************/
#include "UDPsetup.h"

extern const int server_port;
extern const char *LOGIN;
extern const char *QUIT;
#define MAXLINE 256  

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

typedef struct cli{
	//int sock;
	char username[MAXLINE];
	sockaddr client_addr;
	struct cli *next;
}Client;
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
void send_all(Client *head, int sock, const char *msg, sockaddr *clientaddr,socklen_t len, int flag){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(flag && (memcmp(clientaddr,&(cur->client_addr),sizeof(*clientaddr))==0 ))
			continue;
		sendto(sock, msg, strlen(msg),0,&(cur->client_addr),len);
	}
}
int  main(int argc, char const *argv[])
{

	/******sock*************/
	int server_sock = set_sock();
	/******serverinit*************/
	sockaddr_in serveraddr;
	sockaddr clientaddr;
	set_serveraddr(&serveraddr);
	int status = bind(server_sock,(sockaddr *)&serveraddr, sizeof(serveraddr));
	if( status != 0)
		err_ne("bind failure");
	/******recv*************/
	char msg_rece[MAXLINE], msg_send[MAXLINE];
	Client clientlist, *head;
	head = &clientlist;
	head->next = NULL; 
	socklen_t sock_len =sizeof(clientaddr);
	//size_t Client_len = sizeof(clientlist);
    int cnt =0; 
	while(1){
		memset(msg_rece, 0, MAXLINE);

		int length = recvfrom(server_sock, msg_rece, MAXLINE, 0,(sockaddr *)&clientaddr, &sock_len);
		if (length < 0)
			err_ne("recv_msg failure");
		puts(msg_rece);
		msg_rece[length] = 0;
		memset(msg_send, 0, MAXLINE);
		/******login insert*************/
		if(strncmp(msg_rece, LOGIN, strlen(LOGIN)) == 0){
			cnt++;
			Client *this = (Client*)malloc(sizeof(Client));
			set_Client(this, clientaddr, msg_rece+7);
			head_insert(this, head);
			catstr(msg_send, msg_rece+7, "is comming.\n");
			send_all(head, server_sock, msg_send, (sockaddr *)&clientaddr, sock_len,0);
		}else if(strncmp(msg_rece, QUIT, strlen(QUIT)) == 0){
			/******quit delete*************/
			if(--cnt == 0)break;
			Client * sender = find_client(head, clientaddr);
			catstr(msg_send, sender->username, "is leaving.\n");
			delete_client(head, clientaddr);
			send_all(head, server_sock, msg_send, (sockaddr *)&clientaddr,sock_len,0);
		}else{
			/******unicast*************/
			Client *sender = find_client(head, clientaddr);
			if(msg_rece[0] == '@'){
				char name[32], msg[MAXLINE];
				memset(name,0, 32);
				memset(msg, 0, MAXLINE);
				deal_msg(msg_rece, name, msg);
				Client *recer = find_client_byname(head, name);
				if(recer != NULL){
					catstr(msg_send, sender->username, "to you: ");
					strcat(msg_send, msg);
					sendto(server_sock, msg_send, strlen(msg_send),0,&(recer->client_addr), sock_len);	
				}
			}else{
			/******broadast*************/
				catstr(msg_send, sender->username, "saying: ");
				strcat(msg_send, msg_rece);
				send_all(head, server_sock, msg_send, (sockaddr *)&clientaddr,sock_len, 1);
			}
		}	
	}
	return 0;
}