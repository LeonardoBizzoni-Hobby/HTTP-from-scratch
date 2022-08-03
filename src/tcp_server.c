#include "tcp_server.h"

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void tcp_start_server() {

  /* Socket creation */
  int server_socket, client_socket;

  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("[ERROR] failed to create socket.");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "10.83.97.122", &server_addr.sin_addr) <= 0) {
    perror("[ERROR] invalid address.");
    exit(EXIT_FAILURE);
  }

  /* Bind socket to port */
  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("[ERROR] could not bind PORT defined in \"src/tcp_server.h\".");
    exit(EXIT_FAILURE);
  }

  /* Wait for connetion */
  while (1) {
    char message[256];
    printf("Waiting for connection\n");
    listen(server_socket, 5);

    /* Accept client connection */
    client_socket = accept(server_socket, NULL, 0);
    printf("Got a connection\n");

    /* Get and send data to client socket */
    recv(client_socket, &message, sizeof(message), 0);
    printf("Got a message: %s\n", message);
    printf("Sending it back.\n");
    send(client_socket, message, sizeof(message), 0);
    printf("Connection with client ended.\n");
  }

  close(server_socket);
}
