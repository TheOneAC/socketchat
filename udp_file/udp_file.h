#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <openssl/md5.h>
#define MAXLINE 1000
#define SERVER_PORT 6789

typedef struct sockaddr_in sockaddr_in;

void err_msg(const char *msg);
void serv_addr_init(sockaddr_in * serv_sock);
void clit_addr_init(sockaddr_in *clit_sock);
void clit_serv_addr_init(sockaddr_in * clit_serv_sock,const char *ipaddr);
