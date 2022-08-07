#include "http_server.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

Server http_create_server() {
  char host[256];

  /* Find the host name */
  int hostname = gethostname(host, sizeof(host));

  /* Find host information */
  struct hostent *host_entry = gethostbyname(host);

  /* Convert into IP string */
  char *IP = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));

  return (Server){ 8000, IP, "root", "/index.html", "", false };
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
    char data[1024];

    if (srv.verbose) {
      printf("[WAIT] listening on %s:%i\n", inet_ntoa(server_addr.sin_addr), srv.port);
    }
    listen(server_socket, 5);

    /* Accept client connection */
    client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

    if (srv.verbose) {
      printf("[CONN] connection established with %s:%d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
    }

    /* Get data from client socket */
    recv(client_socket, &data, sizeof(data), 0);
    if (srv.verbose) {
      printf("[%s:%d] REQUEST: %s\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port, data);
    }

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
    strcat(request_path, srv.index_name);
  } else {
    strcat(request_path, request_file);
  }

  /* Processing response */
  if (access(request_path, F_OK) == 0) {
    if (strstr(request_type, "GET")) {
      /* HTTP header */
      char *http_header = "HTTP/1.1 200 OK\r\n\n";
      send(client_socket, http_header, strlen(http_header), 0);

      /* HTTP body */
      /* this if will probably have to go */
      if (strstr(request_file, ".png\0") || strstr(request_file, ".jpg\0") || strstr(request_file, ".webp\0") || strstr(request_file, ".jpeg\0") || strstr(request_file, ".mp4\0")) {
        int file;
        struct stat file_stat;

        if ((file = open(request_path, O_RDONLY)) < 0) {
          perror("[FILE_ERROR] can't open requested file\n");
          exit(EXIT_FAILURE);
        }

        fstat(file, &file_stat);
        sendfile(client_socket, file, 0, file_stat.st_size);
        close(file);
      } else {
        FILE *ptr = fopen(request_path, "r");

        /* Read file contents */
        char c;
        size_t size = 0, index = 0;
	char *response = malloc(size+1);

        while ((c = fgetc(ptr)) != EOF) {
          if (index >= size) {
            ++size;
            response = realloc(response, size);
          }
          response[index++] = c;
        }
        response = realloc(response, size + 1);
        response[index] = '\0';

        fclose(ptr);
	send(client_socket, response, strlen(response), 0);
	free(response);
      }
    }
  }
}
