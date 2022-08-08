#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

#include <getopt.h>

#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

typedef struct {
  int port;
  char *addr;
  char *root;
  char *index_name;
  char *out_file;
  bool verbose;
} Server;

Server http_create_server(void);

void *http_start_server(const Server *);
void handle_request(const Server *, const void *, const int);
