#include <stdio.h>
#include "http_server.h"

int main(int argc, char *argv[]) {
  /* Read config file */
  Server srv = http_create_server();
  http_start_server(srv);
  
  return 0;
}
