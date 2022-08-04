#include "http_server.h"
#include <stdio.h>
#include <string.h>

void http_start_server() {
  /* Socket creation */
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);

  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("[ERROR] failed to create socket.");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "10.83.97.122", &server_addr.sin_addr) <= 0) {
    perror("[ERROR] invalid address.");
    exit(EXIT_FAILURE);
  }

  /* Bind socket to port */
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("[ERROR] could not bind PORT defined in \"src/tcp_server.h\".");
    exit(EXIT_FAILURE);
  }

  /* Wait for connetion */
  while (1) {
    char data[512], response[2048] = "";

    printf("\nListening at: %s:%i\n", inet_ntoa(server_addr.sin_addr), PORT);
    listen(server_socket, 5);

    /* Accept client connection */
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    printf("Got a connection with: %s\n", inet_ntoa(client_addr.sin_addr));

    /* Get data from client socket */
    recv(client_socket, &data, sizeof(data), 0);

    /* Handle request and send back data */
    handle_request(&data, &response);
    send(client_socket, response, sizeof(response), 0);

    close(client_socket);
  }
}

/* Data should be used to choose the HTML document requested by the user */
void handle_request(const void *data, void *response) {
  char *http_header = "HTTP/1.1 200 OK\r\n\n";
  char *http_body = "<html>\
<head>\
<title>Hello, World!</title>\
</head>\
<body>\
<h1>Hello, World!</h1>\
</body>\
</html>";

  strcpy(response, http_header);
  strcat(response, http_body);
}
