#include "setup.h"

extern const int server_port;
extern  const size_t MAXLINE;
extern const int MAXCLIENTNUMBER;
extern const char *HELLO;
extern const char *END;

void broadcast(Client* head, int server_sock, const char* msg, sockaddr * clientaddr, socklen_t len ,int flag){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(flag && (memcmp(clientaddr, &(cur->clientaddr), sizeof(*clientaddr)) ==0))
			continue;
		sendto(server_sock, msg, strlen(msg),0,&(cur->clientaddr), len);
	}
}
void receiveFromClient(int server_sock, sockaddr* clientaddr, socklen_t clientlen){
	char message_send[MAXLINE];
	char message_reveive[MAXLINE];
	Client head, *cliheadptr;
	cliheadptr = &head;
	head.next = NULL;
	size_t clilen = sizeof(head);
	for(;;){
		socklen_t len;
		len = clilen;
		memset(message_reveive,0, sizeof(message_reveive));
		int n = recvfrom(server_sock, message_reveive, MAXLINE, 0, clientaddr,&len);
		if(n < 0)
			err("receive message failure");
		printf("meeage being trans :%s",message_reveive);
		message_reveive[n] = 0;
		memset(message_send,0, sizeof(message_send));
		if(strncmp(message_reveive, HELLO, strlen(HELLO) )==0){
			Client *thisclient = (Client*)malloc(sizeof(Client));
			setClient(thisclient, clientaddr,message_reveive+7);
			headInsert(thisclient, cliheadptr);
			catstr(message_send,message_reveive+7, "logined in.\n");

			broadcast(cliheadptr, server_sock, message_send, clientaddr,len, 0);
			//puts(message_reveive);
		}else if(strncmp(message_reveive, END, strlen(END) )==0){
			Client * sender = findClient(cliheadptr, clientaddr);
			catstr(message_send,sender->username, "loged out.\n");
			deleteClient(cliheadptr,clientaddr);
			broadcast(cliheadptr, server_sock, message_send,clientaddr,len,0);
		}else{
			Client * sender = findClient(cliheadptr, clientaddr);
			if(message_reveive[0] == '@'){
				char name[MAXLINE], msg[MAXLINE];
				memset(name,0,sizeof(name));
				memset(msg,0,sizeof(msg));
				dealname(message_reveive,name,msg);
				
				Client* recer = findClientbyname(cliheadptr,name);
				if(recer != NULL){
					catstr(message_send, sender->username, "to you: ");
					strcat(message_send, msg);
					sendto(server_sock, message_send, strlen(message_send),0, &recer->clientaddr, len);
				}
			}else{
				catstr(message_send, sender->username, "saying: ");
				strcat(message_send, message_reveive);
				broadcast(cliheadptr,server_sock,message_send, clientaddr,len,1);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	sockaddr_in serveraddr, clientaddr;
	setupUDPServer(&serveraddr);
	int server_sock = setUDPSocket();
	int status = bind(server_sock, (sockaddr *)&serveraddr, sizeof(serveraddr));
	if( status <0)
		err("bind failure");
	receiveFromClient(server_sock,(sockaddr*)&clientaddr, sizeof(clientaddr));
	return 0;
}