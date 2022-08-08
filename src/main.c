#include "http_server.h"
#include <pthread.h>

int main(int argc, char *argv[]) {
  /* CLI options */
  struct option long_options[] = {
    { "port", required_argument, 0, 'p' },
    { "root", required_argument, 0, 'r' },

    { "index", required_argument, 0, 'i' },
    { "output", required_argument, 0, 'o' },
    { "verbose", no_argument, 0, 'v' },
  };

  /* Read config parameters */
  Server srv = http_create_server();
  char opt;
  int opt_index;

  while ((opt = getopt_long(argc, argv, "p:r:i:o:v", long_options, &opt_index)) != -1) {
    switch (opt) {
    case 'p':
      srv.port = atoi(optarg);

      break;
    case 'r':
      srv.root = optarg;

      break;
    case 'i':
      char name[50];
      snprintf(name, sizeof(name), "/%s", optarg);
      srv.index_name = name;

      break;
    case 'o':
      srv.out_file = optarg;

      break;
    case 'v':
      srv.verbose = true;

      break;
    }
  }

  /* Start http */
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, http_start_server(&srv), NULL);
  /* http_start_server(srv); */
  
  return 0;
}
