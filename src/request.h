#pragma once

#include "http.h"

namespace http {
  struct RequestOpts {
    uint16_t port = 80;
    std::string domain_name;
    std::string host = domain_name;
    std::string query = "/";
    std::string accept = "*/*";
    std::string body = "";

    struct {
      uint8_t major = 1;
      uint8_t minor = 1;
    } http_version;
  };
}  // namespace http
