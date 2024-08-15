#pragma once

#include <expected>
#include <string>

#include "const_definitions.h"
#include "error.h"
#include "http_version.h"

namespace http {
  struct Response {
    http_version version;
    Status status;
    std::string body = "";
    std::unordered_map<std::string, std::string> fields;

  public:
    Response() : version({1,1}), status(Status::NOT_ACCEPTABLE) {}
    ~Response() {}

    static std::expected<Response, Error> build(std::string_view raw_response) noexcept;
  };
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Status &status);
std::ostream &operator<<(std::ostream &os, const http::Response &resp);
