#include "setup.h"

extern const int server_port;
extern  const size_t MAXLINE;
extern const char *server;
extern const char *END;


void sendtoServer(FILE*fp, int client_sock, const sockaddr *serveraddr, socklen_t servlen){
	char message_send[MAXLINE];
	memset(message_send,0,sizeof(MAXLINE));
	int status;
	while(fgets(message_send, MAXLINE, fp) != NULL){
		status = sendto(client_sock, message_send, sizeof(MAXLINE),0, serveraddr, servlen);
		if(status < 0)
			err("send message failure");
		if(strncmp(message_send,END,strlen(END)) == 0)
			return;
	}
}

void receivFromServer(int sock){
	char message_receive[MAXLINE];
	memset(message_receive,0, sizeof(message_receive));
	int status = recvfrom(sock,message_receive, MAXLINE,0, NULL, NULL);
	if(status < 0)
		err("receive message failure");
	fputs(message_receive,stdout);
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
	sendinfo(argv[1],client_sock, (sockaddr*)&serveraddr, sizeof(serveraddr) );
	receivFromServer(client_sock);

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