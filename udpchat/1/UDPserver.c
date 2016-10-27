#include "setup.h"

extern const int server_port;
extern  const size_t MAXLINE;
extern const int MAXCLIENTNUMBER;
extern const char *HELLO;
extern const char *QUIT;

void broadcast(Client* head, int server_sock, const char* msg, sockaddr * clientaddr, socklen_t len ,int flag){
	Client *cur = head->next;
	for(;cur != NULL; cur = cur->next){
		if(flag && (memcmp(clientaddr, &(cur->clientaddr), sizeof(*clientaddr)) ==0))
			continue;
		sendto(server_sock, msg, strlen(msg),0,&(cur->clientaddr), len);
	}
}
void recvFromClient(int server_sock, sockaddr* clientaddr, socklen_t clientlen){
	char msg_send[MAXLINE];
	char msg_recv[MAXLINE];
	Client head, *cliheadptr;
	cliheadptr = &head;
	head.next = NULL;
	size_t clilen = sizeof(head);
	for(;;){
		socklen_t len;
		len = clilen;
		memset(msg_recv,0, sizeof(msg_recv));
		int length = recvfrom(server_sock, msg_recv, MAXLINE, 0, clientaddr,&len);
		if(length < 0)
			err("receive message failure");
		printf("meeage being trans :%s",msg_recv);
		msg_recv[length] = 0;
		memset(msg_send,0, sizeof(msg_send));
		if(strncmp(msg_recv, HELLO, strlen(HELLO) )==0){
			Client *thisclient = (Client*)malloc(sizeof(Client));
			setClient(thisclient, clientaddr,msg_recv+7);
			headInsert(thisclient, cliheadptr);
			catstr(msg_send,msg_recv+7, "logined in.\n");

			broadcast(cliheadptr, server_sock, msg_send, clientaddr,len, 0);
			//puts(msg_recv);
		}else if(strncmp(msg_recv, QUIT, strlen(QUIT) )==0){
			Client * sender = findClient(cliheadptr, clientaddr);
			catstr(msg_send,sender->username, "loged out.\n");
			deleteClient(cliheadptr,clientaddr);
			broadcast(cliheadptr, server_sock, msg_send,clientaddr,len,0);
		}else{
			Client * sender = findClient(cliheadptr, clientaddr);
			if(msg_recv[0] == '>'){
				char name[MAXLINE], msg[MAXLINE];
				//memset(name,0,sizeof(name));
				//memset(msg,0,sizeof(msg));
				bzero(name, MAXLINE);
				bzero(msg, MAXLINE);
				dealname(msg_recv, name, msg);
				printf("meesage_recv :%s", msg_recv);
				//puts("what a bug!!!!!\n");
				printf("meesage :%s\n", msg);
				printf("meesage :%s\n", msg);
				fflush(stdout);
				Client* recer = findClientbyname(cliheadptr,name);
				if(recer != NULL){
					catstr(msg_send, sender->username, "to you: ");
					strcat(msg_send, msg);
					sendto(server_sock, msg_send, strlen(msg_send),0, &recer->clientaddr, len);
				}
			}else{
				catstr(msg_send, sender->username, "saying: ");
				strcat(msg_send, msg_recv);
				broadcast(cliheadptr,server_sock,msg_send, clientaddr,len,1);
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
	recvFromClient(server_sock,(sockaddr*)&clientaddr, sizeof(clientaddr));
	return 0;
}