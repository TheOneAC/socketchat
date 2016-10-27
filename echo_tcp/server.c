/*
 * echo_server
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8090
#define MAXLINE 50

int main(){
  int socket_setup, client_socket, client_length;
  struct sockaddr_in client, server;

  // init tcp socket
  socket_setup = socket(AF_INET, SOCK_STREAM, 0);

  puts("------------Welcome to the Server!-------------------");

  // error check socket init
  if (socket_setup == -1){
    perror("Could not create socket!");
    return -1;
  }

  // init server
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  // bind socket
  if(bind(socket_setup,(struct sockaddr *)&server, sizeof(server)) < 0){
    perror("bind failed");
    return -1;
  }

  // listen for  connection
  listen(socket_setup, 2); //connections limits = 2;

  puts("Waiting for  connections...\n");

  client_length = sizeof(struct sockaddr_in);

  while((client_socket = accept(socket_setup, (struct sockaddr*)&client, (socklen_t*)&client_length)) > 0){
    // connection made
    puts("....Accepted Connection....");
    char message_from_client[MAXLINE];
    char *message_to_client;
    message_to_client = "Enter a message to send : ";
    //sprintf(message_to_client,)
    send(client_socket, message_to_client, strlen(message_to_client),0);
    int rece_len;
    while((rece_len = recv(client_socket, message_from_client, sizeof(message_from_client), 0)) > 0)
    {
      if(rece_len == -1) perror("\nrecv failed");
      //message_from_client[rece_len] = '\0';
      //if(strcmp(message_from_client,"q") == 0)break;
      printf("received message from client %d :  %s",client_socket, message_from_client);
      //puts(message_from_client);
      send(client_socket, message_from_client, sizeof(message_from_client), 0);

      memset(message_from_client, 0, sizeof(message_from_client));

      
    }
    close(client_socket);
  }
  return 0;
}
