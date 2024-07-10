#include "http_server.h"
#include "unistd.h"
#include <stdio.h>

HTTPServer http_create_server(string *server_addr, u16 server_port,
                              const string *root_dir, const string *main_html,
                              const string *log_file, bool verbose_mode) {
  HTTPServer srv = {0};
  Socket socket = mkIPv4Socket(server_addr, server_port, TCP);

  /* Early return in case opening socket failed */
  if (socket.fd <= 0) {
    return srv;
  }

  /* Early return in case listening on socket failed */
  if (listen(socket, 1) == CONNECTION_ERR) {
    return srv;
  }

  srv.log_file = *log_file;
  srv.main_html = *main_html;
  srv.root_dir = *root_dir;
  srv.verbose_mode = verbose_mode;
  srv.socket = socket;

  return srv;
}

void prova(Socket client) {
  sleep(1);
  printf("Hello client!\n");
}

void http_start_server(HTTPServer srv) {
  const string addr = addr2str(srv.socket.addr.address);
  const u16 port = net2host(srv.socket.addr.port);

  /* Wait for connetion */
  while (true) {
    if (srv.verbose_mode) {
      printf("\t[WAIT] listening on %s:%d\n", addr.literal, port);
    }

    accept(&srv.socket, prova);
  }
}
