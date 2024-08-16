#include "response.h"
#include <sys/socket.h>

#include <expected>
#include <iostream>
#include <ranges>
#include <sstream>
#include <utility>

#include "const_definitions.h"

namespace http {
  std::expected<Response, Error> Response::build(std::string_view raw_response) noexcept {
    if (raw_response.empty()) {
      return Err(Error::InvalidResponse);
    }

    Response resp;

    auto lines_view = raw_response | std::views::split(NEW_LINE);
    auto lines_view_iter = lines_view.begin();

    auto words = *lines_view_iter++ | std::views::split(' ');
    auto word_iter = words.begin();
    std::string_view version(*word_iter);
    auto number = version.substr(version.find_first_of('/') + 1);
    auto major = number.substr(0, number.find_first_of('.'));
    auto minor = number.substr(number.find_first_of('.') + 1);

    try {
      resp.version = {.major = (uint8_t)std::stoi(major.data()),
		      .minor = (uint8_t)std::stoi(minor.data())};
    } catch (...) {
      return Err(Error::InvalidHTTPVersion);
    }

    try {
      std::string_view status(*++word_iter);
      resp.status = status_map.at(std::stoi(status.data()));
    } catch (...) {
      return Err(Error::InvalidResponseStatusCode);
    }

    for (std::string_view line; lines_view_iter != lines_view.end() &&
				!(line = std::string_view(*lines_view_iter)).empty();
	 ++lines_view_iter) {
      auto words = line | std::views::split(std::string_view{": "});
      auto it = words.begin();
      std::string field_name((*it).begin(), (*it).end());
      ++it;

      resp.fields[field_name] = std::string((*it).begin(), (*it).end());
    }

    while (++lines_view_iter != lines_view.end()) {
      resp.body += std::string((*lines_view_iter).begin(), (*lines_view_iter).end()) + "\n";
    }

    return resp;
  }

  void Response::send(uint8_t sockfd) {
    std::stringstream ss;
    ss << "HTTP/" << (int)this->version.major << "." << (int)this->version.minor << " " << this->status
       << NEW_LINE;

    for (const auto &field : this->fields) {
      ss << field.first << ": " << field.second << NEW_LINE;
    }

    if (!this->body.empty()) {
      ss << "Content-Length: " << this->body.size() * sizeof(char) << NEW_LINE << NEW_LINE
	 << this->body;
    } else {
      ss << NEW_LINE;
    }

    std::string msg = ss.str();
    ::send((int)sockfd, msg.c_str(), msg.size(), 0);
  }
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Status &status) {
  using namespace http;

  switch (status) {
    case Status::CONTINUE:
      return os << (uint16_t)status << " Continue";
    case Status::SWITCHING_PROTOCOLS:
      return os << (uint16_t)status << " Switching Protocols";
    case Status::PROCESSING:
      return os << (uint16_t)status << " Processing";

    case Status::OK:
      return os << (uint16_t)status << " OK";
    case Status::CREATED:
      return os << (uint16_t)status << " Created";
    case Status::ACCEPTED:
      return os << (uint16_t)status << " Accepted";
    case Status::NON_AUTHORITATIVE_INFORMATION:
      return os << (uint16_t)status << " Non-Authoritative Information";
    case Status::NO_CONTENT:
      return os << (uint16_t)status << " No Content";
    case Status::PARTIAL_CONTENT:
      return os << (uint16_t)status << " Partial Content";
    case Status::ALREADY_REPORTED:
      return os << (uint16_t)status << " Already Reported";
    case Status::IM_USED:
      return os << (uint16_t)status << " IM Used";

    case Status::MULTIPLE_CHOICES:
      return os << (uint16_t)status << " Multiple Choices";
    case Status::MOVED_PERMANENTLY:
      return os << (uint16_t)status << " Moved Permanently";
    case Status::FOUND:
      return os << (uint16_t)status << " Found";
    case Status::SEE_OTHER:
      return os << (uint16_t)status << " See Other";
    case Status::NOT_MODIFIED:
      return os << (uint16_t)status << " Not Modified";
    case Status::USE_PROXY:
      return os << (uint16_t)status << " Use Proxy";
    case Status::UNUSED:
      return os << (uint16_t)status << " Unused";
    case Status::TEMPORARY_REDIRECT:
      return os << (uint16_t)status << " Temporary Redirect";
    case Status::PERMANENT_REDIRECT:
      return os << (uint16_t)status << " Permanent Redirect";

    case Status::BAD_REQUEST:
      return os << (uint16_t)status << " Bad Request";
    case Status::UNAUTHORIZED:
      return os << (uint16_t)status << " Unauthorized";
    case Status::PAYMENT_REQUIRED:
      return os << (uint16_t)status << " Payment Required";
    case Status::FORBIDDEN:
      return os << (uint16_t)status << " Forbidden";
    case Status::NOT_FOUND:
      return os << (uint16_t)status << " Not Found";
    case Status::METHOD_NOT_ALLOWED:
      return os << (uint16_t)status << " Method Not Allowed";
    case Status::NOT_ACCEPTABLE:
      return os << (uint16_t)status << " Not Acceptable";
    case Status::PROXY_AUTHENTICATION_REQUIRED:
      return os << (uint16_t)status << " Proxy Authentication Required";
    case Status::REQUEST_TIMEOUT:
      return os << (uint16_t)status << " Request Timeout";
    case Status::CONFLICT:
      return os << (uint16_t)status << " Conflict";
    case Status::GONE:
      return os << (uint16_t)status << " Gone";
    case Status::LENGTH_REQUIRED:
      return os << (uint16_t)status << " Length Required";
    case Status::PRECONDITION_FAILED:
      return os << (uint16_t)status << " Precondition Failed";
    case Status::PAYLOAD_TOO_LARGE:
      return os << (uint16_t)status << " Payload Too Large";
    case Status::URI_TOO_LONG:
      return os << (uint16_t)status << " URI Too Long";
    case Status::UNSUPPORTED_MEDIA_TYPE:
      return os << (uint16_t)status << " Unsupported Media Type";
    case Status::RANGE_NOT_SATISFIABLE:
      return os << (uint16_t)status << " Range Not Satisfiable";
    case Status::EXPECTATION_FAILED:
      return os << (uint16_t)status << " Expectation Failed";
    case Status::IM_A_TEAPOT:
      return os << (uint16_t)status << " I'm a teapot";
    case Status::MISDIRECTED_REQUEST:
      return os << (uint16_t)status << " Misdirected Request";
    case Status::UNPROCESSABLE_CONTENT:
      return os << (uint16_t)status << " Unprocessable Content";
    case Status::LOCKED:
      return os << (uint16_t)status << " Locked";
    case Status::FAILED_DEPENDENCY:
      return os << (uint16_t)status << " Failed Dependency";
    case Status::TOO_EARLY:
      return os << (uint16_t)status << " Too Early";
    case Status::UPGRADE_REQUIRED:
      return os << (uint16_t)status << " Upgrade Required";
    case Status::PRECONDITION_REQUIRED:
      return os << (uint16_t)status << " Precondition Required";
    case Status::TOO_MANY_REQUESTS:
      return os << (uint16_t)status << " Too Many Requests";
    case Status::REQUEST_HEADER_FIELDS_TOO_LARGE:
      return os << (uint16_t)status << " Request Header Fields Too Large";
    case Status::UNAVAILABLE_FOR_LEGAL_REASONS:
      return os << (uint16_t)status << " Unavailable For Legal Reasons";

    case Status::INTERNAL_SERVER_ERROR:
      return os << (uint16_t)status << " Internal Server Error";
    case Status::NOT_IMPLEMENTED:
      return os << (uint16_t)status << " Not Implemented";
    case Status::BAD_GATEWAY:
      return os << (uint16_t)status << " Bad Gateway";
    case Status::SERVICE_UNAVAILABLE:
      return os << (uint16_t)status << " Service Unavailable";
    case Status::GATEWAY_TIMEOUT:
      return os << (uint16_t)status << " Gateway Timeout";
    case Status::HTTP_VERSION_NOT_SUPPORTED:
      return os << (uint16_t)status << " HTTP Version Not Supported";
    case Status::VARIANT_ALSO_NEGOTIATES:
      return os << (uint16_t)status << " Variant Also Negotiates";
    case Status::INSUFFICIENT_STORAGE:
      return os << (uint16_t)status << " Insufficient Storage";
    case Status::LOOP_DETECTED:
      return os << (uint16_t)status << " Loop Detected";
    case Status::NOT_EXTENDED:
      return os << (uint16_t)status << " Not Extended";
    case Status::NETWORK_AUTHENTICATION_REQUIRED:
      return os << (uint16_t)status << " Network Authentication Required";
    default:
      std::unreachable();
  }
}

std::ostream &operator<<(std::ostream &os, const http::Response &resp) {
  os << "HTTP version: " << (int)resp.version.major << "." << (int)resp.version.minor << "\n";
  os << "Status: " << resp.status;

  for (const auto &entry : resp.fields) {
    os << "\n" << entry.first << ": " << entry.second;
  }

  if (!resp.body.empty()) {
    os << "\nBody: " << resp.body;
  }

  return os;
}
