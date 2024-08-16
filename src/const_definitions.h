#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

#define Err(error) std::unexpected(error)

namespace http {
  constexpr std::string_view NEW_LINE = "\r\n";

  enum class Status {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,

    OK = 200,
    CREATED,
    ACCEPTED,
    NON_AUTHORITATIVE_INFORMATION,
    NO_CONTENT,
    PARTIAL_CONTENT,
    ALREADY_REPORTED,
    IM_USED,

    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    NOT_MODIFIED,
    USE_PROXY,
    UNUSED,
    TEMPORARY_REDIRECT,
    PERMANENT_REDIRECT,

    BAD_REQUEST = 400,
    UNAUTHORIZED,
    PAYMENT_REQUIRED,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    PROXY_AUTHENTICATION_REQUIRED,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PRECONDITION_FAILED,
    PAYLOAD_TOO_LARGE,
    URI_TOO_LONG,
    UNSUPPORTED_MEDIA_TYPE,
    RANGE_NOT_SATISFIABLE,
    EXPECTATION_FAILED,
    IM_A_TEAPOT,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_CONTENT,
    LOCKED,
    FAILED_DEPENDENCY,
    TOO_EARLY,
    UPGRADE_REQUIRED,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT,
    HTTP_VERSION_NOT_SUPPORTED,
    VARIANT_ALSO_NEGOTIATES,
    INSUFFICIENT_STORAGE,
    LOOP_DETECTED,
    NOT_EXTENDED,
    NETWORK_AUTHENTICATION_REQUIRED,
  };

  const std::unordered_map<uint16_t, Status> status_map = {
      {100, Status::CONTINUE},
      {101, Status::SWITCHING_PROTOCOLS},
      {102, Status::PROCESSING},

      {200, Status::OK},
      {201, Status::CREATED},
      {202, Status::ACCEPTED},
      {203, Status::NON_AUTHORITATIVE_INFORMATION},
      {204, Status::NO_CONTENT},
      {205, Status::PARTIAL_CONTENT},
      {208, Status::ALREADY_REPORTED},
      {226, Status::IM_USED},

      {300, Status::MULTIPLE_CHOICES},
      {301, Status::MOVED_PERMANENTLY},
      {302, Status::FOUND},
      {303, Status::SEE_OTHER},
      {304, Status::NOT_MODIFIED},
      {305, Status::USE_PROXY},
      {306, Status::UNUSED},
      {307, Status::TEMPORARY_REDIRECT},
      {308, Status::PERMANENT_REDIRECT},

      {400, Status::BAD_REQUEST},
      {401, Status::UNAUTHORIZED},
      {402, Status::PAYMENT_REQUIRED},
      {403, Status::FORBIDDEN},
      {404, Status::NOT_FOUND},
      {405, Status::METHOD_NOT_ALLOWED},
      {406, Status::NOT_ACCEPTABLE},
      {407, Status::PROXY_AUTHENTICATION_REQUIRED},
      {408, Status::REQUEST_TIMEOUT},
      {409, Status::CONFLICT},
      {410, Status::GONE},
      {411, Status::LENGTH_REQUIRED},
      {412, Status::PRECONDITION_FAILED},
      {413, Status::PAYLOAD_TOO_LARGE},
      {414, Status::URI_TOO_LONG},
      {415, Status::UNSUPPORTED_MEDIA_TYPE},
      {416, Status::RANGE_NOT_SATISFIABLE},
      {417, Status::EXPECTATION_FAILED},
      {418, Status::IM_A_TEAPOT},
      {421, Status::MISDIRECTED_REQUEST},
      {422, Status::UNPROCESSABLE_CONTENT},
      {423, Status::LOCKED},
      {424, Status::FAILED_DEPENDENCY},
      {425, Status::TOO_EARLY},
      {426, Status::UPGRADE_REQUIRED},
      {428, Status::PRECONDITION_REQUIRED},
      {429, Status::TOO_MANY_REQUESTS},
      {431, Status::REQUEST_HEADER_FIELDS_TOO_LARGE},
      {451, Status::UNAVAILABLE_FOR_LEGAL_REASONS},

      {500, Status::INTERNAL_SERVER_ERROR},
      {501, Status::NOT_IMPLEMENTED},
      {502, Status::BAD_GATEWAY},
      {503, Status::SERVICE_UNAVAILABLE},
      {504, Status::GATEWAY_TIMEOUT},
      {505, Status::HTTP_VERSION_NOT_SUPPORTED},
      {506, Status::VARIANT_ALSO_NEGOTIATES},
      {507, Status::INSUFFICIENT_STORAGE},
      {508, Status::LOOP_DETECTED},
      {510, Status::NOT_EXTENDED},
      {511, Status::NETWORK_AUTHENTICATION_REQUIRED},
  };

  struct http_version {
    uint8_t major = 1;
    uint8_t minor = 1;
  };
}  // namespace http
