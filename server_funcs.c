#include "server_funcs.h"

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int PrepareServer(int portno) {
  int sockfd;

  struct sockaddr_in serv_addr;

  fprintf(stderr, "Trying to open socket\n");

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("ERROR opening socket");
    exit(1);
  }

  fprintf(stderr, "Trying to set Socket to SO_REUSEADDR\n");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  /* Initialize socket structure */
  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  fprintf(stderr, "Trying to bind\n");

  /* Now bind the host address using bind() call.*/
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }

  return sockfd;
}

void ForkingServer() {
  fprintf(stderr, "Starting ForkingServer\n");
  socklen_t clilen;
  int sockfd, newsockfd;
  struct sockaddr_in cli_addr;

  sockfd = PrepareServer(5001);

  /* Now start listening for the clients, here
   * process will go in sleep mode and will wait
   * for the incoming connection
   */

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  while (1) {
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }

    /* Create child process */
    int pid = fork();

    if (pid < 0) {
      perror("ERROR on fork");
      exit(1);
    }

    if (pid == 0) {
      /* This is the client process */
      close(sockfd);
      doprocessing(newsockfd);
      exit(0);
    } else {
      close(newsockfd);
    }

  } /* end of while */
}

void MonoServer() {
  fprintf(stderr, "Starting MonoServer\n");

  socklen_t clilen;
  int sockfd, newsockfd;
  struct sockaddr_in cli_addr;

  sockfd = PrepareServer(5001);

  /* Now start listening for the clients, here process will
   * go in sleep mode and will wait for the incoming connection
   */

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  /* Accept actual connection from the client */
  // newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  while (1) {
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    // fprintf(stderr,  "got socket %d\n", newsockfd);
    if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }
    doprocessing(newsockfd);
    // fprintf(stderr,  "processed socket %d\n", newsockfd);

    close(newsockfd);
    // fprintf(stderr,  "closed socket %d\n", newsockfd);
  }

  fprintf(stderr, "could not write to socket\n");

  if (newsockfd < 0) {
    perror("ERROR on accept");
    exit(1);
  }
}

void doprocessing(int sock) {
  int n;
  char buffer[33];
  bzero(buffer, 33);
  n = read(sock, buffer, 32);

  if (n < 0) {
    perror("ERROR reading from socket");
    exit(1);
  }

  //  fprintf(stderr, "Here is the message: %s\n", buffer);

  if (strstr(buffer, "foo") != NULL) {
    if (strstr(buffer, "bar") != NULL) {
      __builtin_trap();
    }
  }

  //  n = write(sock, "I got your message", 18);

  if (n < 0) {
    fprintf(stderr, "could not write to socket\n");
    // perror("ERROR writing to socket");
    // exit(1);
  }
}
