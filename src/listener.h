#pragma once

#include <netinet/in.h>

#include <cstdint>
#include <expected>
#include <functional>
#include <string>
#include <unordered_map>

#include "error.h"
#include "request.h"
#include "response.h"

namespace http {
  struct Listener {
  public:
    ~Listener() {}

    static inline std::expected<Listener, Error> on_local(uint16_t port, uint16_t backlog = 4096) {
      return on(port, INADDR_LOOPBACK, backlog);
    }

    static inline std::expected<Listener, Error> on_LAN(uint16_t port, uint16_t backlog = 4096) {
      return on(port, INADDR_ANY, backlog);
    }

    std::expected<void, Error> serve();
    std::expected<void, Error> serve_async();

  private:
    Listener();

    static std::expected<Listener, Error> on(uint16_t port, in_addr_t addr,
					     uint16_t backlog = 4096);

  public:
    std::unordered_map<std::string, std::function<Response(const Request &)>> routes;

  private:
    int8_t socketfd;
    struct sockaddr_in addr;
  };
}  // namespace http
