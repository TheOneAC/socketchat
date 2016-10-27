/*  TCP echo_client  single conennction
 * zeroarn
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXLINE 50
#define PORT 8090
#define SERVERIP "127.0.0.1" // change to test remote ip address of server

int main(){
  int clientfd;
  struct sockaddr_in serveraddr;

  puts("--------------Welcome to the Client Connection Program!-----------------");

  // init socket
  clientfd = socket(AF_INET, SOCK_STREAM, 0);

  // check the socket init
  if (clientfd == -1){
    perror("Failed to init socket!");
    return -1;
  }

  // init serveraddr address and port
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
  serveraddr.sin_port = htons(PORT);

  // connect to remote server
  if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
    perror("Failed to connect to remote server");
    return -1;
  }

  // inform user that they are connected to the remote server successfully
  printf("Connected to server: %s on Port: %d\n", SERVERIP, PORT);

  char message_get[MAXLINE];
  char message_put[MAXLINE];
  // receive initial specification from remote serveraddr
  if (recv(clientfd, message_get, sizeof(message_get), 0) < 0){
     perror("Failed to receive message!");
    return -1;
  }
  // print out  info message
  printf("%s\n", message_get);
  while(1){
      printf("please input new message : ");
      //scanf ("%[^\n]%*c", message_put);
      fgets(message_put,sizeof(message_put),stdin);
      // send message to remote server
      if(strlen(message_put) == 1){
        printf("blank message!\n");
        continue;
      }
      // quit
      if(strlen(message_get) == 2 && message_get[0] == 'q')break;
      if (send(clientfd, message_put, sizeof(message_put), 0) < 0){
        perror("Failed to send message!");
        return -1;
      }
      
      // receive echo  message from server
      memset(message_get,0,sizeof(message_get));  
      int recv_len;    
      recv_len =  recv(clientfd, message_get, sizeof(message_get), 0);
      if ( recv_len < 0){
        perror("Failed to receive message!");
        return -1;
      }else if(recv_len == 1 && message_get[0] == '\0')continue;
      printf("Message echo from the serveraddr: %s", message_get);

  }

  // destroy socket
  close(clientfd);

  // logout the server
  printf("Good Bye\n-----------Thank you for using this program!-------------\n");
  return 0;
}
