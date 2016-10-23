#include "setup.h"

extern const int PORT; 
extern const char *SERVERAADR; 
extern const int MAXLINE; 
#define CONNECT_LIMIT 10

#define SOURCE_ID 'A'
#define TARGET_ID 'C'
/* change SOURCE_ID id and TARGET_ID for every new client [A~Z]
 * if TARGET_ID is 'a' which means 'all', then it will breadcast to all the client
 */
/*********thread for receive********/
void* client_receive(void*arg){
	int status;
	int client_fd = *(int *)arg;
	char message_receive[MAXLINE];
	int message_len;
	while(1)
	{
		memset(message_receive,0,sizeof(message_receive));
		message_len = recv(client_fd,message_receive,sizeof(message_receive),0);
		if(message_len < 0){
			perror("fail to receive message");
			break;
		}
		/*******deal with source ID*****/
		char from_cid = message_receive[strlen(message_receive)-1];
		message_receive[strlen(message_receive)-1] = '\0';
		
		printf("\nmeesage from [%c]client : %s\n$$ > :",from_cid, message_receive);
		fflush(stdout);
	}
	exit(0);
}
/*********thread for send********/
void* client_send(void* arg){
	int client_fd = (*(int *)arg);
	char message_send[MAXLINE];
	int status;
	while(1)
	{
		memset(message_send,0,sizeof(message_send));
		fgets(message_send,sizeof(message_send),stdin);
		int len = strlen(message_send)-1;// exception for '\n' in the last
		/*******test for quit and empty*****/
		if(len == 0){
			puts("Empty message!");
			continue;
		}
		else if(len == 1 && message_send[0] == 'q')break;
		/*******ADD SOURCE_ID and target ID and send*****/
		//message_send[len++]= SOURCE_ID;//source id
		message_send[len++]= TARGET_ID;//target id
		if(send(client_fd,message_send,len,0) < 0){
			perror("fail to send message");
			break;
		} 
		printf("$$ > :");
	}
	exit(0);
}

int main(int argc, char const *argv[])
{
	/*******init client_fd and server address*****/
	int status;
	int client_fd = SetTCPSock();
	struct sockaddr_in server;
	SetTCPServer(&server);
	/*******connect to server *****/
	status = connect(client_fd,(struct sockaddr*)&server,sizeof(server));
	errn(status,"fail to init connect to server");
	puts("login success!");
	char client_info[MAXLINE];
	client_info[0] = SOURCE_ID;
	status = send(client_fd,client_info,MAXLINE,0);
	errn(status,"send client_info faliure");
	printf("$$ > :");
	/*******threads for read and write *****/
	pthread_t rthread, wthread;
	status = pthread_create(&wthread,NULL, &client_send,(void*)&client_fd);
	errnz(status,"create rthread failure");
	status = pthread_create(&rthread,NULL, &client_receive,(void*)&client_fd);
	errnz(status,"create wthread failure");
	status = pthread_join(wthread,NULL);
	errnz(status,"join wthread failure");
	status = pthread_join(rthread,NULL);
	errnz(status,"join rthread failure");
	close(client_fd);
	return 0;
}