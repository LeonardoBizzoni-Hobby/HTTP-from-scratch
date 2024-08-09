#pragma once

#include <cstdint>
#include <string_view>

namespace http {
  struct http_version{
    uint8_t major = 1;
    uint8_t minor = 1;
  };

  struct RequestOpts {
    uint16_t port = 80;
    std::string_view domain_name;
    std::string_view host = domain_name;
    std::string_view query = "/";
    std::string_view accept = "*/*";
    std::string_view body = "";

    http_version version;
  };
}  // namespace http
