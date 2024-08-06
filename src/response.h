#pragma once

namespace http {
  enum class Status {
    OK = 200,
    NOT_FOUND = 404,
  };

  struct Response {
    Status status;
  };
}  // namespace http
