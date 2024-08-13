#pragma once

#include <future>
#include "error.h"
#include "method.h"
#include "request.h"
#include "response.h"

#define ERR(error) std::unexpected(error)
#define NEW_LINE std::string_view("\r\n")

namespace http {
  std::expected<Response, Error> send(Method, const RequestOpts& req);

  std::expected<int8_t, Error> connect(const std::string_view& domain_name, const uint16_t port = 80);

  std::string build_request(Method method, const RequestOpts &req);
  std::expected<std::string, Error> read_raw_response(const int8_t socketfd);

  namespace async {
    std::future<std::expected<Response, Error>> send(Method method, const RequestOpts &req);
  }
}  // namespace http
