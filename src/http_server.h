#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  int port;
  char *addr;
  char *root;
} Server;

Server http_create_server(void);
void http_start_server(const Server);
void handle_request(const Server, const void *, const int);
