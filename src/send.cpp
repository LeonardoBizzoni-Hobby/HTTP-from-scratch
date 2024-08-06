#include <iostream>
#include <sstream>
#include <unordered_map>

#include "http.h"

#define BUFFSIZE 1024

static std::unordered_map<std::string, struct addrinfo> ip_map;

namespace http {
  std::expected<int8_t, Error> connect_to(const std::string &domain_name, const uint16_t port) {
    struct addrinfo hints = {0}, *addr_list;
    hints.ai_family = AF_UNSPEC;      // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP only

    if (getaddrinfo(domain_name.c_str(), std::to_string(port).c_str(), &hints, &addr_list)) {
      return ERR(Error::DNSResolution);
    }

    int8_t remote_socketfd;
    if (auto it = ip_map.find(domain_name); it != ip_map.end()) {
      remote_socketfd =
	  socket(it->second.ai_family, it->second.ai_socktype, it->second.ai_protocol);
      if (remote_socketfd < 0) {
	close(remote_socketfd);
	return ERR(Error::SocketConnection);
      }
    } else {
      struct addrinfo *remote = 0;
      for (struct addrinfo *addr = addr_list; addr; addr = addr->ai_next) {
	remote_socketfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (remote_socketfd < 0) {
	  close(remote_socketfd);
	  continue;
	}

	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	if (setsockopt(remote_socketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0 ||
	    setsockopt(remote_socketfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof timeout) < 0) {
	  close(remote_socketfd);
	  continue;
	}

	// Connected with remote server
	if (connect(remote_socketfd, addr->ai_addr, addr->ai_addrlen) < 0) {
	  close(remote_socketfd);
	} else {
	  remote = addr;
	  break;
	}
      }

      if (!remote) {
	freeaddrinfo(addr_list);
	return ERR(Error::ServerNotFound);
      }

      ip_map[domain_name] = *remote;
      freeaddrinfo(addr_list);
    }

    return remote_socketfd;
  }

  std::expected<Response, Error> sendreq(Method method, const RequestOpts &req) {
    auto maybe_socketfd = connect_to(req.domain_name, req.port);

    if (!maybe_socketfd.has_value()) {
      return ERR(maybe_socketfd.error());
    }

    std::stringstream ss;
    ss << method << " " << req.query << " HTTP/" << (int)req.http_version.major << "."
       << (int)req.http_version.minor << NEW_LINE;
    ss << "Host: " << req.host << NEW_LINE;
    ss << "Accept: " << req.accept << NEW_LINE;

    if (!req.body.empty()) {
      ss << "Content-Length: " << req.body.length() << NEW_LINE << NEW_LINE << req.body;
    } else {
      ss << NEW_LINE;
    }

    std::string msg = ss.str();
    std::cout << "Request:\n===================\n" << msg << "\n===================\n" << std::endl;
    send((int)maybe_socketfd.value(), msg.c_str(), msg.size(), 0);

    msg = "";
    char buffer[BUFFSIZE] = "";
    ssize_t bytes_read = 0;
    while ((bytes_read = read((int)maybe_socketfd.value(), buffer, BUFFSIZE - 1)) > 0) {
      if (bytes_read == -1) {
	std::cerr << "\t\tError while reading!" << std::endl;
      }

      buffer[bytes_read] = '\0';
      msg += buffer;
    }

    std::cout << "Response:\n===================\n" << msg << "\n===================" << std::endl;

    close(maybe_socketfd.value());
    return Response();
  }
}  // namespace http
