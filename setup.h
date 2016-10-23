#ifndef _TCP_SETUP
#define _TCP_SETUP

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct sockaddr_in sockaddr_in;
void SetTCPServer(sockaddr_in *serverAddr);
int SetTCPSock();
void errn(const int status, const char *detail);
void errnz(const int status, const char *detail);

#endif