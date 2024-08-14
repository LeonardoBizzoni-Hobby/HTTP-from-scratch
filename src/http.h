#pragma once

#include <future>

#include "error.h"
#include "method.h"
#include "request.h"
#include "response.h"
#include "listener.h"

#define ERR(error) std::unexpected(error)
#define NEW_LINE std::string_view("\r\n")

namespace http {
  std::expected<Response, Error> send(Method, const Request& req);

  std::expected<int8_t, Error> connect(const std::string_view& domain_name, const uint16_t port = 80);

  std::string build_request(Method method, const Request &req);
  std::expected<std::string, Error> read_raw_message(const uint8_t socketfd);

  namespace async {
    std::future<std::expected<Response, Error>> send(Method method, const Request &req);
  }
}  // namespace http
