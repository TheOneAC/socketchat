#ifndef _UDP_SETUP
#define _UDP_SETUP

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

void err(const char *msg);
void catstr(char *str, const char *strc, const char *strn);
void inputErr(const char* msg, const char* detail);
void setupUDPServer(sockaddr_in* serverAddr);
void setupUDPClient(sockaddr_in* clientAddr);
int setUDPSocket();
void sendinfo(const char* username, int sock, const sockaddr *servaddr, socklen_t servlen );

typedef struct ClientNode{
	sockaddr clientaddr;
	char username[128];
	struct ClientNode *next;
}Client;

void setClient(Client *cur, const sockaddr *addrptr, const char *name);
void headInsert(Client *cur, Client* head);
Client* findClient(Client* head, const sockaddr *addrptr);
void deleteClient(Client * head, const sockaddr* addrptr);
Client* findClientbyname(Client* head, const char* name);
void dealname(const char *buffer, char *name, char *msg);

#endif