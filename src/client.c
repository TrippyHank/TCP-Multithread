//
// client.c - TCP Client
//
// Copyright (c) 2020 OpSocket <opsocket@protonmail.com>
//

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>

#define __KB 1024
#define PORT 8080
#define SA struct sockaddr

// Console output
#define CHECK "\e[32m\xE2\x9C\x94\e[0m"
#define ERROR "\e[31m\xE2\x9C\x98\e[0m"

// Reset the memory block with zeros
#define zeros(block) memset(&block, 0, sizeof(block));

int main() {
  int sockfd; 
  struct sockaddr_in servaddr;
  char str[INET_ADDRSTRLEN];

  // Socket creation and verification 
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sockfd == -1) { 
    printf(ERROR" Socket creation failed.\n"); 
    exit(0); 
  } else
    printf(CHECK" Socket successfully created.\n"); 

  // Reset memory block with zeros
  zeros(servaddr);

  // Assign IP, PORT 
  servaddr.sin_family = AF_INET; 
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
  servaddr.sin_port = htons(PORT); 

  // connect the client socket to server socket 
  if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
    printf(ERROR" Connection with server failed...\n"); 
    exit(0); 
  } else
    printf(CHECK" Connected to server (%s:%d).\n", 
      inet_ntop(AF_INET, &servaddr.sin_addr, str, sizeof(str)), PORT);

  char rbuff[__KB];
  while(1) {
    printf("CLIENT> ");
    // Read characters from stream
    fgets(rbuff, __KB, stdin);
    // Write buffer to socket file descriptor
    if(write(sockfd, &rbuff, sizeof(rbuff)) == -1) { 
      perror("write"); 
    }
    // Reset memory block with zeros
    zeros(rbuff);
  }

  // Close socket file descriptor
  close(sockfd);
  return 0;
} 