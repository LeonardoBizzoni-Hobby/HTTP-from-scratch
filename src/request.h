#pragma once

#include <cstdint>
#include <string_view>
#include <ostream>
#include <unordered_map>

namespace http {
  struct http_version {
    uint8_t major = 1;
    uint8_t minor = 1;
  };

  struct Request {
    uint16_t port = 80;
    std::string_view domain_name;
    std::string_view query = "/";
    std::string_view body = "";

    http_version version = {.major = 1, .minor = 1};

    std::unordered_map<std::string, std::string> optheaders = {};
  };
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Request &req);
