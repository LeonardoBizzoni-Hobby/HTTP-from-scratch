#include "http_server.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Server http_create_server() {
  char host[256];

  int hostname = gethostname(host, sizeof(host));   // find the host name
  struct hostent *host_entry = gethostbyname(host); // find host information
  char *IP = inet_ntoa(*(
      (struct in_addr *)host_entry->h_addr_list[0])); // Convert into IP string

  return (Server){8000, IP, "/home/leo/Dev/mockServerRoot"};
}

void http_start_server(const Server srv) {
  /* Socket creation */
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);

  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("[ERROR] failed to create socket.");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(srv.port);
  if (inet_pton(AF_INET, srv.addr, &server_addr.sin_addr) <= 0) {
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
    char data[512];

    printf("\nListening at: %s:%i\n", inet_ntoa(server_addr.sin_addr),
           srv.port);
    listen(server_socket, 5);

    /* Accept client connection */
    client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    printf("Got a connection with: %s\n", inet_ntoa(client_addr.sin_addr));

    /* Get data from client socket */
    recv(client_socket, &data, sizeof(data), 0);
    printf("Request: %s\n", data);

    /* Handle request and send back data */
    handle_request(srv, &data, client_socket);
    close(client_socket);
  }

  close(server_socket);
}

/* Data should be used to choose the HTML document requested by the user */
void handle_request(const Server srv, const void *data,
                    const int client_socket) {
  /* Analyzing request */
  char request_path[100];

  char *str = (char *)data;
  char *request_type = strsep(&str, " ");
  char *request_file = strsep(&str, " ");

  strcpy(request_path, srv.root);
  if (strcmp(request_file, "/") == 0) {
    strcat(request_path, "/index.html");
  } else {
    strcat(request_path, request_file);
  }

  /* Processing response */
  char *response;

  if (access(request_path, F_OK) == 0) {
    if (strstr(request_type, "GET")) {
      /* HTTP header */
      char *http_header = "HTTP/1.1 200 OK\r\n\n";
      size_t size = strlen(http_header);
      response = malloc(size);

      strcpy(response, http_header);

      /* HTTP body */
      FILE *ptr = fopen(request_path, "r");

      /* Read file contents */
      char c;
      int len = strlen(response);
      while ((c = fgetc(ptr)) != EOF) {
	if (len >= size) {
	  ++size;
	  response = realloc(response, size);
	}
	response[len++] = c;
      }
      response = realloc(response, size+1);
      response[len] = '\0';

      fclose(ptr);
    }

  } else {
    char *http_header = "HTTP/1.1 404 ERR\r\n\n";
    char *http_body = "<html>\
<head>\
<title>File not found!</title>\
</head>\
<body>\
<h1>Couldn't find file!</h1>\
</body>\
</html>";

    strcpy(response, http_header);
    strcat(response, http_body);
  }

  send(client_socket, response, strlen(response), 0);

  free(response);
}
