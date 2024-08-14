#pragma once

namespace http {
  enum class Error {
    SocketCreation,
    SocketBind,
    SocketListen,
    SocketConnection,
    DNSResolution,
    ServerNotFound,
    InvalidRead,
    InvalidResponse,
    InvalidResponseHTTPVersion,
    InvalidResponseStatusCode,
  };
}
