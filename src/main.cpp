#include <unistd.h>

#include <iostream>

#include "http.h"
#include "request.h"

int main() {
  auto resp1 = http::async::send(http::Method::GET,
				 http::RequestOpts{.domain_name = "google.com", .query = "/prova"});

  auto resp = resp1.get();
  if (!resp.has_value()) {
    switch (resp.error()) {
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
      case http::Error::InvalidRead: {
	std::cout << "Invalid read" << std::endl;
      } break;
    }
  } else {
    std::cout << resp.value() << std::endl;
  }
}
