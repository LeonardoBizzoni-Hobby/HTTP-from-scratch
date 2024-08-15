#include <iostream>

#include "http.h"
#include "request.h"
#include "response.h"

int main() {
  auto server = http::Listener::on_local(8800);
  server.value().routes["/"] = [](const http::Request &req) -> http::Response {
    std::cout << req << std::endl;
    return http::Response();
  };
  server.value().routes["/prova"] = [](const http::Request &req) -> http::Response {
    std::cout << "Hello, World!" << std::endl;
    auto _ = req.body;
    return http::Response();
  };
  server.value().serve();
}
