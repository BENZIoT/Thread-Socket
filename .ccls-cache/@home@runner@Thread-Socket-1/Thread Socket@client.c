// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8081

char msg[1024] = "";
int indexChar = 0;

int main(int argc, char const *argv[]) {

  for (int i = 1; i < argc; i++) {
    strcat(msg, argv[i]);
    if (i != argc - 1) {
      strcat(msg, " ");
    }
  }
  printf("echoClient %s\n", msg);

  for (int i = 0; i < strlen(msg); i++) {
    if (msg[i] == ' ') {
      indexChar = i;
      break;
    }
  }

  int status, valread, client_fd;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary
  // form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                        sizeof(serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }
  send(client_fd, msg, strlen(msg), 0);
  valread = read(client_fd, buffer,
                 1024 - 1); // subtract 1 for the null
                            // terminator at the end
  printf("echo \"%s\"\n", buffer);

  // closing the connected socket
  close(client_fd);
  return 0;
}