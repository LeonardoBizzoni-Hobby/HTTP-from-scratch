#pragma once

#include <cstdint>

namespace http {
  struct http_version {
    uint8_t major = 1;
    uint8_t minor = 1;
  };
}  // namespace http
