#pragma once

namespace http {
  enum class Error {
    SocketCreation,
    SocketBind,
    SocketListen,
    SocketConnection,
    DNSResolution,
    ServerNotFound,
    InvalidResponse,
    InvalidRequest,
    InvalidRequestMethod,
    InvalidRequestPath,
    InvalidQueryRequest,
    InvalidHTTPVersion,
    InvalidResponseStatusCode,
  };
}
