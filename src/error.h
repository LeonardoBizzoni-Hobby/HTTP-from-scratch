#pragma once

namespace http {
  enum class Error {
    SocketCreation,
    SocketConnection,
    DNSResolution,
    ServerNotFound,
    InvalidRead,
    InvalidResponse,
    InvalidResponseHTTPVersion,
    InvalidResponseStatusCode,
  };
}
