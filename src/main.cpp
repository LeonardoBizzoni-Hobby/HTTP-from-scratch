#include <unistd.h>

#include <iostream>

#include "http.h"
#include "request.h"

int main() {
  auto resp1 =
      http::async::send(http::Method::GET, http::RequestOpts{.domain_name = "example.com",
							     .accept = "application/json"});
  // auto resp1 = http::sendreq(http::Method::POST, {.domain_name = "example.com", .body = "Hello,
  // World!"});

  std::cout << "Going to sleep" << std::endl;
  sleep(1);
  std::cout << "I woke up" << std::endl;

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
