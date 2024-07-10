#ifndef CORE_HTTP_SERVER_H
#define CORE_HTTP_SERVER_H

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#include "socket.h"
#include "type.h"

typedef struct {
  Socket socket;
  string root_dir;
  string main_html;
  string log_file;
  bool verbose_mode;
} HTTPServer;

typedef struct {
  bool valid;
  char *method;
  char *path;
  char *args;
  char *body;
} Request;

HTTPServer http_create_server(string *server_addr, u16 server_port,
                              const string *root_dir, const string *main_html,
                              const string *log_file, bool verbose_mode);

void http_start_server(HTTPServer);
Request parse_request(const HTTPServer *, const void *, const int);
void process_response(Request *, const int, const HTTPServer *);

#endif
