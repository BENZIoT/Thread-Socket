// socket server example, handles multiple clients using threads

#include <arpa/inet.h> //inet_addr
#include <pthread.h>   //for threading , link with lpthread
#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen
#include <sys/socket.h>
#include <unistd.h> //write

int numOfTheads[50];
int countTheads = 0;

// the thread function
void *connection_handler(void *);
void remove_newline(char *str);

int main(int argc, char *argv[]) {
  int socket_desc, client_sock, c, *new_sock;
  struct sockaddr_in server, client;

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    printf("Could not create socket");
  }
  // puts("Socket created");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8081);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    // print the error message
    perror("bind failed. Error");
    return 1;
  }
  // puts("bind done");

  // Listen
  listen(socket_desc, 3);

  // Accept and incoming connection
  // puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  c = sizeof(struct sockaddr_in);
  while (client_sock =
             accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)) {
    // puts("Connection accepted");

    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = client_sock;

    if (pthread_create(&sniffer_thread, NULL, connection_handler,
                       (void *)new_sock) < 0) {
      perror("could not create thread");
      return 1;
    }

    // puts("Handler assigned");
  }

  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }
  return 0;
}
/*
  This will handle connection for each client
  */
void *connection_handler(void *socket_desc) {
  // Get the socket descriptor
  int sock = *(int *)socket_desc;
  int n;
  
  // printf("socker is %d\n", sock);
  char str_numOfTheads[8];
  sprintf(str_numOfTheads, "%d", numOfTheads[sock] = countTheads++);

  send(sock, str_numOfTheads, strlen(str_numOfTheads), 0);

  printf("[%s] send: \"%s\" (%ld bytes)\n", str_numOfTheads, str_numOfTheads,
         sizeof(str_numOfTheads[numOfTheads[sock]]) + 1);
  // numOfTheads++;

  char sendBuff[100], client_message[2000];

  while ((n = recv(sock, client_message, 2000, 0)) > 0) {
    remove_newline(client_message);
    printf("[%d] recv: \"%s\" (%d bytes)\n", numOfTheads[sock], client_message, n);
    printf("[%d] send: \"%s\" (%d bytes)\n\n", numOfTheads[sock], client_message, n);
    send(sock, client_message, n, 0);
    
    memset(client_message, '\0', sizeof(client_message)); 
  }
  close(sock);

  if (n == 0) {
    printf("\nClient[%d] Disconnected!!!\n", numOfTheads[sock]);
    // --countTheads;
  } else {
    perror("recv failed");
  }
  return 0;
}

void remove_newline(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}