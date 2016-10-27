/******UDPclient*************/
#include "UDPsetup.h"

extern const int server_port;
#define MAXLINE 256
extern const char *LOGIN;
extern const char *QUIT;


sockaddr_in servaddr;
void *send_msg(void *arg){
	int len, client_sock = *(int*)arg;
	char msg_send[MAXLINE];
	memset(msg_send, 0 ,MAXLINE);
	while(fgets(msg_send, MAXLINE,stdin) != NULL){
		len = sendto(client_sock, msg_send, strlen(msg_send), 0,(sockaddr*)&servaddr, sizeof(servaddr));
		if(len < 0)
			err_ne("send msg failed");
		if(strncmp(msg_send, QUIT, strlen(QUIT)) == 0)
			return NULL;
	}
}
void *rece_msg(void *arg){
	//pthread_detach()
	char msg_rev[MAXLINE];
	int len, client_sock = *(int*)arg;
	while(1){
		memset(msg_rev, 0, MAXLINE);
		len = recvfrom(client_sock, msg_rev, MAXLINE, 0, NULL, NULL);
		if (len < 0)
			err_ne("recvfrom server failure");
		fputs(msg_rev, stdout);
	}
}
int  main(int argc, char const *argv[])
{
	/******error run*************/
	if(argc != 2){
		fputs("Run like this: ./client zero\n",stdout);
		exit(1);
	}
	/******sock*************/
	int client_sock = set_sock();
	/******serverinit*************/
	
	client_set_server(&servaddr);
	/******login*************/
	send_login(argv[1], client_sock, (sockaddr*)&servaddr, sizeof(servaddr));
	
	/******thread send*************/
	pthread_t sthread, rthread;
	int status = pthread_create(&sthread, NULL, (void*)&send_msg, (void*)&client_sock);
	if(status != 0)
		err_ne("send thread create failure");
	/******thread recv*************/
	status = pthread_create(&rthread, NULL, (void*)&rece_msg, (void*)&client_sock);
	if(status != 0)
		err_ne("rece thread create failure");
	status = pthread_join(sthread,NULL);
	if(status != 0)
		err_ne("send thread join failure");
	/*status = pthread_join(rthread,NULL);
	if(status != 0)
		err_ne("rece thread join failure");
	*/
	//close(client_sock);
	return 0;
}