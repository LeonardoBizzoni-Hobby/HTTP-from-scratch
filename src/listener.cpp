#include "listener.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>

#include "error.h"
#include "http.h"

namespace http {
  Listener::Listener() : routes({}), socketfd(::socket(AF_INET, SOCK_STREAM, 0)), addr({}) {}

  std::expected<Listener, Error> Listener::on(uint16_t port, in_addr_t addr, uint16_t backlog) {
    Listener server;

    uint32_t opt = 1;
    if (server.socketfd < 0 || ::setsockopt(server.socketfd, SOL_SOCKET,
					    SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
      return ERR(Error::SocketCreation);
    }

    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = htonl(addr);
    server.addr.sin_port = htons(port);

    if (::bind(server.socketfd, (struct sockaddr *)&server.addr, sizeof(server.addr)) < 0) {
      return ERR(Error::SocketBind);
    }

    if (::listen(server.socketfd, backlog) < 0) {
      return ERR(Error::SocketListen);
    }

    return server;
  }

  std::expected<void, Error> Listener::serve() {
    socklen_t addrsize = sizeof(this->addr);
    int8_t clientfd = ::accept(this->socketfd, (struct sockaddr *)&this->addr, &addrsize);
    if (clientfd < 0) {
      return ERR(Error::SocketConnection);
    }

    auto raw_req = read_raw_message(clientfd);
    if (raw_req.has_value()) {
      std::cout << raw_req.value() << std::endl;
    }

    ::shutdown(clientfd, SHUT_RDWR);
    ::close(clientfd);
    return {};
  }
}  // namespace http
