#include "setup.h"

const int PORT = 6784;
const int MAXLINE = 100;
#define SERVERAADR  "127.0.0.1"


void SetTCPServer(sockaddr_in *serverAddr){
	/*server->sin_family = AF_INET;
	server->sin_addr.s_addr = INADDR_ANY;
	server->sin_port = htons(PORT);*/
	serverAddr->sin_family = AF_INET;
	serverAddr->sin_port = htons(PORT);
	serverAddr->sin_addr.s_addr = inet_addr(SERVERAADR);
}
int SetTCPSock(){
	int sock =socket(AF_INET,SOCK_STREAM,0);
	if(sock <0){
		perror("server socket init failure");
		return -1;
	}
	return sock;
}
void errn(const int status, const char *detail){
	if(status < 0){
		perror(detail);
		exit(-1);
	}
	else return ;
}
void errnz(const int status, const char *detail){
	if(status != 0){
		perror(detail);
		exit(-1);
	}
	else return;
}