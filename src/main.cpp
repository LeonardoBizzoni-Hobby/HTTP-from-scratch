#include <iostream>

#include "http.h"

int main() {
  auto resp1 = http::async::send(http::Method::GET, {.domain_name = "example.com",
						     .version = {1, 1}});

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
      case http::Error::InvalidResponse: {
	std::cout << "Invalid response" << std::endl;
      } break;
      case http::Error::InvalidResponseHTTPVersion: {
	std::cout << "Invalid response http version" << std::endl;
      } break;
      case http::Error::InvalidResponseStatusCode: {
	std::cout << "Invalid response status code" << std::endl;
      } break;
    }
  } else {
    std::cout << resp.value() << std::endl;
  }
}
