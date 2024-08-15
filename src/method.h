#pragma once

#include <ostream>
#include <unordered_map>
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

  const std::unordered_map<std::string, Method> method_map = {
      {"get", Method::GET},	    {"head", Method::HEAD},	{"post", Method::POST},
      {"put", Method::PUT},	    {"delete", Method::DELETE}, {"connect", Method::CONNECT},
      {"options", Method::OPTIONS}, {"trace", Method::TRACE},	{"patch", Method::PATCH},
      {"update", Method::UPDATE},
  };
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Method &method);
