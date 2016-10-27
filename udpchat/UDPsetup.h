#ifndef _UDP_setup
#define _UDP_setup

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/socket.h>


typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
#define MAXLINE 256

void err_ne(const char* err_msg);
int set_sock();
void set_serveraddr(sockaddr_in *serveraddr );
void client_set_server(sockaddr_in *servaddr);
void catstr(char *str, const char* stri, const char* strn);
void send_login(const char *username,int sock, sockaddr* servaddr, socklen_t servlen);

typedef struct cli{
	//int sock;
	char username[MAXLINE];
	sockaddr client_addr;
	struct cli *next;
}Client;

void set_Client(Client *this, sockaddr clientaddr, const char *name);
void head_insert(Client *this, Client *head);
Client *find_client_byname(Client *head, const char *name);
Client *find_client(Client *head, sockaddr client_addr);
void delete_client(Client *head, sockaddr client_addr);
void deal_msg(const char *buffer, char *name, char *msg);


#endif