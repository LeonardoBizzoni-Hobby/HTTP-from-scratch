#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <string_view>
#include <unordered_map>

#include "error.h"
#include "request.h"

namespace http {
  enum class Status {
    PARSE_ERROR,
    OK = 200,
    NOT_FOUND = 404,
  };

  static std::unordered_map<uint16_t, Status> status_map = {
      {200, Status::OK},
      {404, Status::NOT_FOUND},
  };

  struct Response {
    http_version version;
    Status status;
    std::string body = "";
    std::unordered_map<std::string, std::string> fields;

  public:
    static std::expected<Response, Error> build(std::string_view raw_response) noexcept;
    ~Response() {}

  private:
    Response() {}
  };
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Status &status);
std::ostream &operator<<(std::ostream &os, const http::Response &resp);
