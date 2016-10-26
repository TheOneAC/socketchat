#include "UDPsetup.h"

const int server_port = 6789;
const char *server = "127.0.0.1";
const int MAXLINE = 256;
const char *LOGIN = "@LOGIN";
const char *QUIT = "@QUIT";

void err_ne(cosnt char* err_msg){
	perror(err_msg);
	exit(1);
}

int set_sock(){
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
		err_ne("init socket failure");
	return sock;
}
void set_serveraddr(sockaddr_in *serveraddr ){
	memset(serveraddr,0,sizeof(*serveraddr));
	serveraddr->sin_family = AF_INET;
	serveraddr->sin_port = htons(server_port);
	serveraddr->sin_addr.s_addr = htonl(INADDR_ANY);
}
void client_set_server(sockaddr_in *servaddr){
	memset(servaddr,0,sizeof(*servaddr));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(server_port);
	inet_pton(AF_INET, server, &(servaddr->sin_addr));
}
void catstr(char *str, const char* stri, const char* strn){
	strcat(str, stri);
	strcat(str, " ");
	strcat(str, strn);
}
void send_login(const char *username,int client_sock, (sockaddr*)&servaddr, socklen_t servlen){
	char msg_send[maxline];
	memset(msg_send, 0, sizeof(msg_send));
	catstr(msg_send, LOGIN, username);
	int status = sendto(client_sock, msg_send,sizeof(msg_send), 0, servaddr, servlen);
	if(status < 0)
		err_ne("sengdlogin failure");
}