/*  TCP echo_lancher  single conennction
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
#define waiterIP "127.0.0.1" // change to test  ip address of waiter

int main(){
  int lancherfd;
  struct sockaddr_in waiteraddr;

  puts("-------------we had lanch the chat program!-----------------");

  // init socket
  lancherfd = socket(AF_INET, SOCK_STREAM, 0);

  // check the socket init
  if (lancherfd == -1){
    perror("Failed to init socket!");
    return -1;
  }

  // init waiteraddr address and port
  waiteraddr.sin_family = AF_INET;
  waiteraddr.sin_addr.s_addr = inet_addr(waiterIP);
  waiteraddr.sin_port = htons(PORT);

  // connect to  waiter
  if (connect(lancherfd, (struct sockaddr *)&waiteraddr, sizeof(waiteraddr)) < 0){
    perror("Failed to connect to  waiter");
    return -1;
  }

  // inform user that they are connected to the  waiter successfully
  //printf("Connected to waiter: %s on Port: %d\n", waiterIP, PORT);

  char message_receive[MAXLINE];
  char message_send[MAXLINE];
  
  puts("********let`s we chat***********");
  while(1){
      printf("$ > :");
      
      fgets(message_send,sizeof(message_send),stdin);
      // send message to  waiter
      if(strlen(message_send) == 1){
        continue;
      }
      // quit
      if(strlen(message_receive) == 2 && message_receive[0] == 'q')break;
      if (send(lancherfd, message_send, sizeof(message_send), 0) < 0){
        perror("Failed to send message!");
        return -1;
      }
      
      // receive message from waiter
      memset(message_receive,0,sizeof(message_receive));  
      int recv_len;    
      recv_len =  recv(lancherfd, message_receive, sizeof(message_receive), 0);
      if ( recv_len < 0){
        perror("Failed to receive message!");
        return -1;
      }else if(recv_len == 2 && message_receive[0] == '\0')continue;
      printf("* < %s", message_receive);

  }

  // destroy socket
  close(lancherfd);

  // logout the waiter
  printf("Good Bye\n-----------Thank you for using this program!-------------\n");
  return 0;
}
