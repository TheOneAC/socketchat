#include "setup.h"

extern const int server_port;
extern  const size_t MAXLINE;
extern const char *server;
extern const char *QUIT;


void sendtoServer(FILE*fp, int client_sock, const sockaddr *serveraddr, socklen_t servlen){
	char msg_send[MAXLINE];
	memset(msg_send,0,sizeof(MAXLINE));
	int status;
	while(fgets(msg_send, MAXLINE, fp) != NULL){
		status = sendto(client_sock, msg_send, sizeof(MAXLINE),0, serveraddr, servlen);
		if(status < 0)
			err("send message failure");
		if(strncmp(msg_send,QUIT,strlen(QUIT)) == 0)
			return;
	}
}

void receivFromServer(int sock){
	char msg_recv[MAXLINE];
	memset(msg_recv,0, sizeof(msg_recv));
	int status = recvfrom(sock,msg_recv, MAXLINE,0, NULL, NULL);
	if(status < 0)
		err("receive message failure");
	fputs(msg_recv,stdout);
	return;
}
int main(int argc, char const *argv[])
{
	if(argc >2 || argc == 1)
		inputErr("Please Input A Parameter","Username");
	sockaddr_in serveraddr;
	//setupUDPServer(&serveraddr);
	setupUDPClient(&serveraddr);
	int client_sock = setUDPSocket();
	/************ info for login **********/
	sendinfo(argv[1],client_sock, (sockaddr*)&serveraddr, sizeof(serveraddr) );
	receivFromServer(client_sock);
	/**********************/
	pid_t processID;
	processID = fork();
	if(processID<0)
		err("fork() failed");
	else if(processID>0){
		sendtoServer(stdin, client_sock, (sockaddr *) &serveraddr, sizeof(serveraddr));
	}else{
		while(1)
			receivFromServer(client_sock);
	}
	exit(0);
}