#include "method.h"

#include <utility>

std::ostream &operator<<(std::ostream &os, const http::Method &method) {
  using namespace http;

  switch (method) {
    case Method::GET: {
      return os << "GET";
    } break;
    case Method::HEAD: {
      return os << "HEAD";
    } break;
    case Method::POST: {
      return os << "POST";
    } break;
    case Method::PUT: {
      return os << "PUT";
    } break;
    case Method::DELETE: {
      return os << "DELETE";
    } break;
    case Method::CONNECT: {
      return os << "CONNECT";
    } break;
    case Method::OPTIONS: {
      return os << "OPTIONS";
    } break;
    case Method::TRACE: {
      return os << "TRACE";
    } break;
    case Method::PATCH: {
      return os << "PATCH";
    } break;
    case Method::UPDATE: {
      return os << "UPDATE";
    } break;
    default:
      std::unreachable();
  }
}
