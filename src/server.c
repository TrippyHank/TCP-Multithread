//
// server.c - Multi-threaded TCP Server 
//
// Copyright (c) 2020 OpSocket <opsocket@protonmail.com>
//

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <stdint.h>

#define __KB 1024             // The length limit for the buffer
#define HOST "127.0.0.1"      // INADDR_ANY to allow any IP
#define PORT "8080"           // The port to listen for connections
#define SA struct sockaddr    // Struct that holds the socket information
#define N_THREADS 2           // Read & Write threads per connection handler

// Console output
#define CHECK "\e[32m\xE2\x9C\x94\e[0m"
#define ERROR "\e[31m\xE2\x9C\x98\e[0m"

// Reset the memory block with zeros
#define zeros(block) memset(&block, 0, sizeof(block));

/**
 * Listen for TCP connections on host:port
 * @param  hostname The hostname to listen on
 * @param  servname The port / service to listen on
 * @param  addrlen  The length of socket-address
 * @return          The listening socket file descriptor
 */
int tcp_listen(const char *hostname, const char *servname, socklen_t *addrlen) {
  int sockfd, err;
  socklen_t len = 1;
  struct addrinfo hints, *res, *__res;

  // Reset memory block with zeros
  zeros(hints);

  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((err = getaddrinfo(hostname, servname, &hints, &res)) != 0)
    printf(ERROR" tcp_listen on %s:%s error: %s", hostname, servname, gai_strerror(err));

  __res = res;

  do {
    // Create a socket
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) { 
      perror("socket"); 
      continue; 
    } else printf(CHECK " Socket successfully created.\n");

    // Set socket options before it is bound
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &len, sizeof(len)) == -1) {
      perror("setsockopt");
    } else printf(CHECK " Socket options have been set.\n");

    // Bind socket to IP address
    if ((bind(sockfd, res->ai_addr, res->ai_addrlen)) != 0) { 
      perror("bind");
      close(sockfd);
    } else {
      printf(CHECK " Socket successfully bound.\n"); 
      break;
    }
  } while((res = res->ai_next) != NULL);

  if (res == NULL)
    printf("tcp_listen on %s:%s error", hostname, servname);

  // Socket is ready to listen for connections
  if ((listen(sockfd, 5)) == -1) { perror("listen"); } 
  else printf("Server is listening on %s:%s..\n", hostname, servname);

  // Set pointer value for size of protocol address
  if (addrlen) *addrlen = res->ai_addrlen;

  freeaddrinfo(__res);

  return(sockfd);
}

/**
 * The thread read handler
 * @param  arg The connection socket file descriptor
 */
void *r_thread(void *arg) {
  // Detach the thread
  pthread_detach(pthread_self());

  int connfd = *((int *) arg);

  // Read buffer
  char rbuff[__KB];

  // @FIXME: Read logic here, should be an infinite loop that:
  // (1) Blocks while reading from a file descriptor (i.e read(connfd, rbuff, sizeof(rbuff)))
  // (2) Display the read buffer
  // (3) Reset the read buffer

  // Close socket file descriptor
  close(connfd);

  // Terminate the calling thread
  pthread_exit(NULL);
}

/**
 * The thread write handler
 * @param  arg The connection socket file descriptor
 */
void *w_thread(void *arg) {
  int connfd = *((int *) arg);

  // Detach the thread
  pthread_detach(pthread_self());

  // Write buffer
  char wbuff[__KB];

  // @FIXME: Write logic here, should be an infinite loop that:
  // (1) Blocks while reading characters from stream i.e fgets(wbuff, __KB, stdin)
  // (2) Write the buffer to a file descriptor
  // (3) Reset the write buffer

  // Close socket file descriptor
  close(connfd);

  // Terminate the calling thread
  pthread_exit(NULL);
}

/**
 * The thread connection handler
 * @param  arg The thread handler argument
 */
void *conn_thread(void *arg) {
  pthread_t threads[N_THREADS];

  // Detach the thread
  pthread_detach(pthread_self());

  // Create read thread
  if(pthread_create(&threads[0], NULL, r_thread, arg) == -1) {
    perror("r_thread");
  }

  // Create write thread
  if(pthread_create(&threads[1], NULL, w_thread, arg) == -1) {
    perror("w_thread");
  }

  // Terminate the calling thread
  pthread_exit(NULL);
}

int main(void) {
  int sockfd, *connfd;
  pthread_t thread_id;
  socklen_t addrlen, len;
  struct sockaddr_in client;

  // Create a TCP listening socket
  sockfd = tcp_listen(HOST, PORT, &addrlen);

  while(1) {
    len = addrlen;
    // Accept connections on the socket
    if ((*connfd = accept(sockfd, (SA *)&client, &len)) == -1) { 
      perror("accept"); continue;
    }
    // Print incoming connection informations
    char ip4[INET_ADDRSTRLEN];
    printf(CHECK " Server: got connection from %s\n", 
      inet_ntop(AF_INET, (SA *)&client.sin_addr.s_addr, ip4, INET_ADDRSTRLEN));

    // Create a thread to handle connection
    if(pthread_create(&thread_id, NULL, conn_thread, connfd) == -1) {
      perror("thread");
    }
  }

  // Close socket file descriptor
  close(sockfd);
  return 0;
}