#include "http_server.h"

static char g_log[100];

#define SaveLog()                                                              \
  {                                                                            \
    if (strcmp(srv->out_file, "") != 0) {                                      \
      FILE *ptr;                                                               \
      ptr = fopen(srv->out_file, "a");                                         \
                                                                               \
      if (ptr != NULL) {                                                       \
        fputs(g_log, ptr);                                                     \
        fclose(ptr);                                                           \
      }                                                                        \
    }                                                                          \
  }

Server http_create_server() {
  char host[256];

  /* Find the host name */
  int hostname = gethostname(host, sizeof(host));

  /* Find host information */
  struct hostent *host_entry = gethostbyname(host);

  /* Convert into IP string */
  char *IP = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));

  return (Server){8000, IP, "root", "/index.html", "", false};
}

void *http_start_server(const Server *srv) {
  /* Socket creation */
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);

  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    strcpy(g_log, "[ERROR] failed to create socket.\n");
    perror(g_log);
    SaveLog();
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(srv->port);
  if (inet_pton(AF_INET, srv->addr, &server_addr.sin_addr) <= 0) {
    strcpy(g_log, "[ERROR] invalid address.\n");
    perror(g_log);
    SaveLog();

    exit(EXIT_FAILURE);
  }

  /* Bind socket to port */
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    strcpy(g_log, "[ERROR] could not bind PORT defined in \"src/tcp_server.h\".\n");
    perror(g_log);
    SaveLog();

    exit(EXIT_FAILURE);
  }

  /* Wait for connetion */
  while (1) {
    char data[1024];
    char tmp_buf[7];

    if (srv->verbose || strcmp(srv->out_file, "") != 0) {
      strcpy(g_log, "[WAIT] listening on ");
      strcat(g_log, inet_ntoa(server_addr.sin_addr));

      snprintf(tmp_buf, sizeof(tmp_buf), ":%d", srv->port);
      strcat(g_log, tmp_buf);
      strcat(g_log, "\n");

      if (srv->verbose) {
        printf("%s", g_log);
      }
      SaveLog();
    } else {
      printf("Listening on %s:%d\n", inet_ntoa(server_addr.sin_addr), srv->port);
    }

    listen(server_socket, 20);

    /* Accept client connection */
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

    if (srv->verbose || strcmp(srv->out_file, "") != 0) {
      strcpy(g_log, "[CONN] connection established with ");
      strcat(g_log, inet_ntoa(client_addr.sin_addr));

      snprintf(tmp_buf, sizeof(tmp_buf), ":%d", client_addr.sin_port);
      strcat(g_log, tmp_buf);
      strcat(g_log, "\n");

      if (srv->verbose) {
        printf("%s", g_log);
      }
      SaveLog();
    }

    /* Get data from client socket */
    recv(client_socket, &data, sizeof(data), 0);
    if (srv->verbose || strcmp(srv->out_file, "") != 0) {
      strcpy(g_log, "[");
      strcat(g_log, inet_ntoa(client_addr.sin_addr));
      strcat(g_log, tmp_buf);
      strcat(g_log, "] REQUEST: ");
      strcat(g_log, data);
      strcat(g_log, "\n");

      if (srv->verbose) {
        printf("%s", data);
      }
      SaveLog();
    }

    /* Handle request and send back data */
    Request req = parse_request(srv, &data, client_socket);

    if (req.valid) {
      process_response(&req, client_socket, srv);
    }

    close(client_socket);
  }

  close(server_socket);
}

Request parse_request(const Server *srv, const void *data, const int client_socket) {
  char request_path[100];

  char *str = (char *)data;
  if (strlen(str) < 10) {
    /* Error 500 */
    strcpy(g_log, "[REQ_ERROR] request not properly formatted\n");
    perror(g_log);
    SaveLog();
    return (Request){false};
  }

  char *request_type = strsep(&str, " ");
  char *request_args = strsep(&str, " ");
  char *request_file = strsep(&request_args, "?");

  strcpy(request_path, srv->root);
  if (strcmp(request_file, "/") == 0) {
    strcat(request_path, srv->index_name);
  } else {
    strcat(request_path, request_file);
  }

  return (Request) {true, request_type, request_path, request_args, str};
}

void process_response(Request *req, const int client_socket, const Server *srv) {
  if (access(req->path, F_OK) == 0) {
    /* HTTP header */
    char *http_header = "HTTP/1.1 200 OK\r\n\n";
    send(client_socket, http_header, strlen(http_header), 0);

    signal(SIGPIPE, SIG_IGN); // ignoring the broken pipe signal

    if (strstr(req->method, "GET")) {
      printf("\t%s %s?%s", req->method, req->path, req->args);

    } else if (strstr(req->method, "POST")) {
      req->args = malloc(100);

      do {
	req->args = "";
	req->args = strsep(&(req->body), "\n");
      }while (strstr(req->args, "&") == NULL);

      printf("\t%s %s\t%s", req->method, req->path, req->args);
    }

    /* HTTP body */
    int file;
    struct stat file_stat;

    if ((file = open(req->path, O_RDONLY)) < 0) {
      strcpy(g_log, "[FILE_ERROR] can't open requested file\n");
      perror(g_log);
      SaveLog();
      return;
    }

    fstat(file, &file_stat);
    sendfile(client_socket, file, 0, file_stat.st_size);
    close(file);

    printf(ANSI_COLOR_GREEN " 200 OK" ANSI_COLOR_RESET "\n");
  } else {
    printf(ANSI_COLOR_RED " 404 Not Found" ANSI_COLOR_RESET "\n");

    char *http_header = "HTTP/1.1 404 ERR\r\n\n";
    send(client_socket, http_header, strlen(http_header), 0);
  }
}
