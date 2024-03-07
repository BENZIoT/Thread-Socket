// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8081

int indexChar = 0;

char *makeWord(char *c, int start, int end);

void getName(char *c, int start, int end);
void getEchomsg(char *c, int start, int end);

char *subString(char *msg, int start, int end);

int main(int argc, char const *argv[]) {
  int server_fd, new_socket;
  ssize_t valread;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  for (;;) {
    char buffer[1024] = {0};
    if (listen(server_fd, 3) < 0) {
      perror("listen");
      exit(EXIT_FAILURE);
    }
    if ((new_socket =
             accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024 - 1); // subtract 1 for the null

    for (int i = 0; i < strlen(buffer); i++) {
      if (buffer[i] == ' ') {
        indexChar = i;
        break;
      }
    }

    char *name = subString(buffer, 0, indexChar);
    char *echomsg = subString(buffer, indexChar + 1, strlen(buffer));

    char logFile[1024] = { 0 };

    strcat(logFile, name);
    strcat(logFile, ": ");
    strcat(logFile, echomsg);

    // write massge to file
    FILE *fptr = fopen("echo.txt", "a+");
    // fprintf(fptr, "%s: %s\n", name, echomsg);
    fprintf(fptr, "%s\n", logFile);
    fclose(fptr);

    // send echo massge back to client
    send(new_socket, echomsg, strlen(echomsg), 0);

    // debug
    // printf("In Comeing msg: %s\n", buffer);
    // printf("Name: %s\n", name);
    // printf("Echo: %s\n", echomsg);
    // printf("LOG file: %s\n", logFile);

    // deallocate
    free(name);
    free(echomsg);

    // closing the connected socket
    close(new_socket);
  }
  // closing the listening socket
  close(server_fd);
  return 0;
}

char *makeWord(char *c, int start, int end) {
  char *s = malloc(end - start + 1);
  if (!s)
    return NULL;
  int k;
  for (k = 0; k < end - start; k++) {
    s[k] = c[start + k];
  }
  s[k] = '\0';
  return s;
}

char *subString(char *msg, int start, int end) {
  if (msg == NULL || start < 0 || end < start)
    return NULL;

  int length = end - start;
  char *sub = (char *)malloc(length + 1); // +1 for null terminator
  if (sub == NULL)
    return NULL;

  memcpy(sub, msg + start, length);
  sub[length + 1] = '\0';

  return sub;
}