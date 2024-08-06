#pragma once

#include "http.h"

namespace http {
  struct RequestOpts {
    const std::string domain_name;
    const uint16_t port = 80;
    const std::string path = "/";
    const std::string body = "";
  };
}
