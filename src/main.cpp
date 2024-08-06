#include <iostream>

#include "http.h"

int main() {
  auto resp1 = http::sendreq(http::Method::GET, {.domain_name = "example.com"});
  // auto resp1 = http::sendreq(http::Method::POST, {.domain_name = "example.com", .body = "Hello, World!"});

  if (!resp1.has_value()) {
    switch (resp1.error()) {
      case http::Error::SocketCreation: {
	std::cout << "Socket creation" << std::endl;
      } break;
      case http::Error::SocketConnection: {
	std::cout << "Socket connection" << std::endl;
      } break;
      case http::Error::DNSResolution: {
	std::cout << "DNS resolution" << std::endl;
      } break;
      case http::Error::ServerNotFound: {
	std::cout << "Server not found" << std::endl;
      } break;
    }
  }
}
