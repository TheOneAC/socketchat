/*
 * echo_waiter
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8090
#define MAXLINE 50

int main(){
  int socket_setup, lancher_socket, lancher_length;
  struct sockaddr_in lancher, waiter;

  // init tcp socket
  socket_setup = socket(AF_INET, SOCK_STREAM, 0);
  // error check socket init
  if (socket_setup == -1){
    perror("Could not create socket!");
    return -1;
  }

  // init waiter
  waiter.sin_family = AF_INET;
  waiter.sin_addr.s_addr = INADDR_ANY;
  waiter.sin_port = htons(PORT);

  // bind socket
  if(bind(socket_setup,(struct sockaddr *)&waiter, sizeof(waiter)) < 0){
    perror("bind failed");
    return -1;
  }

  // listen for  connection
  listen(socket_setup, 2); //connections limits = 2;

  puts("Waiting for  connections to chat...");

  lancher_length = sizeof(struct sockaddr_in);

  while((lancher_socket = accept(socket_setup, (struct sockaddr*)&lancher, (socklen_t*)&lancher_length)) > 0){
    // connection made
    puts("********let`s we chat***********");
    char message_from_lancher[MAXLINE];
    char message_to_lancher[MAXLINE];
  
    int rece_len;
    while((rece_len = recv(lancher_socket, message_from_lancher, sizeof(message_from_lancher), 0)) > 0)
    {
      if(rece_len == -1) perror("\nrecv failed");
      //message_from_lancher[rece_len] = '\0';
      //if(strcmp(message_from_lancher,"q") == 0)break;
      printf("* < %s", message_from_lancher);
      //puts(message_from_lancher);
      printf("$ > :");

      fgets(message_to_lancher,sizeof(message_to_lancher),stdin);
      if(strlen(message_to_lancher) == 1){
        continue;
      }
      send(lancher_socket, message_to_lancher, sizeof(message_to_lancher), 0);

      memset(message_from_lancher, 0, sizeof(message_from_lancher));

      
    }
    close(lancher_socket);
  }
  return 0;
}
