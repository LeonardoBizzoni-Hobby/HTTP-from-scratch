#pragma once

#include <cstdint>
#include <expected>
#include <ostream>
#include <string_view>
#include <unordered_map>

#include "error.h"
#include "const_definitions.h"
#include "method.h"

namespace http {
  struct Request {
  public:
    static std::expected<Request, Error> build(std::string_view raw_response) noexcept;

  public:
    uint16_t port = 80;
    std::string_view domain_name;
    std::string path = "/";
    std::string query;
    std::string body;

    Method method;
    http_version version = {.major = 1, .minor = 1};

    std::unordered_map<std::string, std::string> optheaders = {};
  };
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Request &req);
