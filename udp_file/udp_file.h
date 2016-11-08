#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <time.h>
#include <openssl/md5.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>


#define MAXLINE 1024
#define SERVER_PORT 6789

typedef struct sockaddr_in sockaddr_in;

void err_msg(const char *msg);
void serv_addr_init(sockaddr_in * serv_sock);
void clit_addr_init(sockaddr_in *clit_sock);
void clit_serv_addr_init(sockaddr_in * clit_serv_sock,const char *ipaddr);
void send_filename(char *filename,int clit_sock,sockaddr_in *clit_serv_addr);
void pack_res(int clit_sock,sockaddr_in *clit_serv_addr);
void pack_ack(int serv_sock, sockaddr_in *clit_addr);
void Gen_md5(const char* filename, char* md5_buffer);