#pragma once

#include <ostream>
namespace http {
  enum class Method {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UPDATE,
  };
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Method &method);
