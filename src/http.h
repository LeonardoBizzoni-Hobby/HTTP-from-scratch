#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdint>
#include <expected>
#include <string_view>

#include "error.h"
#include "method.h"
#include "request.h"
#include "response.h"

#define ERR(error) std::unexpected(error)
#define NEW_LINE std::string_view("\r\n")

namespace http {
  std::expected<Response, Error> sendreq(Method, const RequestOpts& req);

  std::expected<int8_t, Error> connect_to(const std::string_view& domain_name, const uint16_t port = 80);

  std::string build_request(Method method, const RequestOpts &req);
  std::expected<std::string, Error> read_raw_response(const int8_t socketfd);
}  // namespace http
