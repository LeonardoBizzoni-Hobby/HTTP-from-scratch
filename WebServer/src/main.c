#include <getopt.h>
#include <pthread.h>
#include <stdlib.h>

#include <http.h>

int main(int argc, char *argv[]) {
  /* CLI options */
  struct option long_options[] = {
      {"addr", required_argument, 0, 'a'},
      {"port", required_argument, 0, 'p'},
      {"root", required_argument, 0, 'r'},

      {"index", required_argument, 0, 'i'},
      {"output", required_argument, 0, 'o'},
      {"verbose", no_argument, 0, 'v'},
  };

  /* Read config parameters */
  u16 server_port = 8080;
  bool verbose_mode = false;

  string log_file = mkstring("log.txt");
  string server_addr = mkstring("localhost");
  string root_dir = mkstring(ROOTDIR "/root/");
  string main_html = mkstring("index.html");

  i8 opt;
  while ((opt = getopt_long(argc, argv, "p:r:i:o:v", long_options, 0)) != -1) {
    switch (opt) {
    case 'a': {
      server_addr = mkstring(optarg);
    } break;
    case 'p': {
      server_port = atoi(optarg);
    } break;
    case 'r': {
      root_dir = mkstring(optarg);
    } break;
    case 'i': {
      main_html = mkstring(optarg);
    } break;
    case 'o': {
      log_file = mkstring(optarg);
    } break;
    case 'v': {
      verbose_mode = true;
    } break;
    }
  }

  HTTPServer srv = http_create_server(&server_addr, server_port, &root_dir,
                                      &main_html, &log_file, verbose_mode);
  http_start_server(srv);

  /* Start http */
  /* pthread_t thread_id; */
  /* pthread_create(&thread_id, NULL, http_start_server(&srv), NULL); */

  return 0;
}
