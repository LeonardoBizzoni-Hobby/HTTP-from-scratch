#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
  int port;
  char *addr;
  char *root;
  char *index_name;
  char *out_file;
  bool verbose;
} Server;

typedef struct {
  bool valid;
  char *method;
  char *path;
  char *args;
} Request;

Server http_create_server(void);

void *http_start_server(const Server *);
Request parse_request(const Server *, const void *, const int);
void process_response(const Request *, const int, const Server *);
