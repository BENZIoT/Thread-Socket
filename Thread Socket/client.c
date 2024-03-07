#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 50

int numOfTheads = 0;

void remove_newline(char *str);

int main() {
  int sock_desc, valread;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  char sbuff[MAX_SIZE], rbuff[MAX_SIZE];

  if ((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    printf("Failed creating socket\n");

  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8081);

  if (connect(sock_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    printf("Failed to connect to server\n");
    return -1;
  }

  // if(recv(sock_desc,rbuff,MAX_SIZE,0)==0)
  //    printf("Error");
  //   else
  //    fputs(rbuff,stdout);

  valread = read(sock_desc, buffer,
                 1024 - 1); // subtract 1 for the null
                            // terminator at the end
  // printf("%s\n", buffer);
  numOfTheads = atoi(buffer);

  printf("[%d] Enter message: ", numOfTheads);
  while (fgets(sbuff, MAX_SIZE, stdin) != NULL) {

    send(sock_desc, sbuff, strlen(sbuff), 0);
    remove_newline(sbuff);
    printf("[%d] send: \"%s\" (%ld bytes)\n", numOfTheads, sbuff,
           strlen(sbuff));

    if (recv(sock_desc, rbuff, MAX_SIZE, 0) == 0)
      printf("Error");
    else {
      remove_newline(sbuff);
      // fputs(rbuff, stdout);
      printf("[%d] recv: \"%s\" (%ld bytes)\n", numOfTheads, sbuff,
             strlen(sbuff));
    }
    printf("\n[%d] Enter message: ", numOfTheads);

    bzero(rbuff, MAX_SIZE);

    if (!strcmp(sbuff, "exit")) {
      break;
    }
  }
  close(sock_desc);
  return 0;
}

void remove_newline(char *str) {
  int len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
}