#include "tcp_client.h"

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void tcp_run_client() {
  /* Socket creation */
  int network_socket, conversion_value;
  char recv_buffer[256], message[256] = "helloooooooooo";
  struct in6_addr server_ip;

  if ((network_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("[ERROR] failed to create socket.");
    exit(EXIT_FAILURE);
  }

  /* Define server socket */
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "10.83.97.122", &server_addr.sin_addr) <= 0) {
    perror("[ERROR] Invalid address");
    exit(EXIT_FAILURE);
  }

  /* Connect to server socket */
  if (connect(network_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("[ERROR] failed to enstablish connection with server.");
    exit(EXIT_FAILURE);
  }

  /* Get data from server */
  printf("Connected to server!\nSending message %s.\n", message);
  send(network_socket, &message, sizeof(message), 0);
  printf("Message sent!\n");
  recv(network_socket, &recv_buffer, sizeof(recv_buffer), 0);
  printf("Server says: %s\n", recv_buffer);

  close(network_socket);
}
