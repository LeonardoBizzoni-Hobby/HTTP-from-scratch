#include <iostream>

#include "http.h"

int main() {
  auto server = http::Listener::create_on_local(8800);
  server.value().serve();

  server.value().routes["prova"] = [](const http::Request &req) { std::cout << req << std::endl; };

  http::Request request;
  server.value().routes["prova"](request);
}
