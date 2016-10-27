/******UDPserver*************/
#include "UDPsetup.h"

extern const int server_port;
extern const char *LOGIN;
extern const char *QUIT;
//#define MAXLINE 256  


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
	Client clientlist, *head = NULL;
	head = &clientlist;
	head->next = NULL; 
	socklen_t sock_len =sizeof(clientaddr);
	//size_t Client_len = sizeof(clientlist);
	while(1){
		memset(msg_rece, 0, MAXLINE);
		int length = recvfrom(server_sock, msg_rece, MAXLINE, 0,(sockaddr *)&clientaddr, &sock_len);
		if (length < 0)
			err_ne("recv_msg failure");
		printf("message being transfer : %s",msg_rece);
		msg_rece[length] = 0;
		memset(msg_send, 0, MAXLINE);
		/******login insert*************/
		if(strncmp(msg_rece, LOGIN, strlen(LOGIN)) == 0){
			Client *this = (Client*)malloc(sizeof(Client));
			set_Client(this, clientaddr, msg_rece+7);
			head_insert(this, head);
			catstr(msg_send, msg_rece+7, "is comming.\n");
			puts(msg_send);
			send_all(head, server_sock, msg_send, (sockaddr *)&clientaddr, sock_len,0);
		}else if(strncmp(msg_rece, QUIT, strlen(QUIT)) == 0){
			/******quit delete*************/
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
