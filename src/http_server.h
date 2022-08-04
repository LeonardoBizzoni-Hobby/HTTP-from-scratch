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

void http_start_server();
void handle_request(const Server, const void *, void *);

