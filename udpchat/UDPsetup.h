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

void err_ne(const char* err_msg);
int set_sock();
void set_serveraddr(sockaddr_in *serveraddr );
void client_set_server(sockaddr_in *servaddr);
void catstr(char *str, const char* stri, const char* strn);
void send_login(const char *username,int sock, sockaddr* servaddr, socklen_t servlen);


#endif