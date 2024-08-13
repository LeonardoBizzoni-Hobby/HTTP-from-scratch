#pragma once

#include <cstdint>
#include <expected>
#include <functional>
#include <string>
#include <unordered_map>

#include "error.h"
#include "request.h"

namespace http {
  struct Listener {
  public:
    ~Listener() {}

    static std::expected<Listener, Error> create_on_local(uint16_t port);
    static std::expected<Listener, Error> create_on_lan(uint16_t port);

    void serve();
    void serve_async();

  private:
    Listener() : routes({}) {}

  public:
    std::unordered_map<std::string, std::function<void(const Request &)>> routes;
  };
}  // namespace http
