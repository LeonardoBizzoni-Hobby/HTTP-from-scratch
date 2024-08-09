#include <iostream>
#include <ranges>
#include <string_view>

#include "http.h"

namespace http {
  std::expected<Response, Error> Response::build(std::string_view raw_response) noexcept {
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
      resp.version = {.major = 0, .minor = 0};
    }

    try {
      std::string_view status(*++word_iter);
      resp.status = status_map[std::stoi(status.data())];
    } catch (...) {
      resp.status = Status::PARSE_ERROR;
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
}  // namespace http

std::ostream &operator<<(std::ostream &os, const http::Status &status) {
  switch (status) {
    case http::Status::PARSE_ERROR:
      return os << "parse error";
    case http::Status::OK:
      return os << "ok";
    case http::Status::NOT_FOUND:
      return os << "not found";
    default:
      return os;
  }
}

std::ostream &operator<<(std::ostream &os, const http::Response &resp) {
  os << "HTTP version: " << (int)resp.version.major << "." << (int)resp.version.minor << "\n";
  os << "Status: " << resp.status;

  for (const auto entry : resp.fields) {
    os << "\n" << entry.first << ": " << entry.second;
  }

  if (!resp.body.empty()) {
    os << "\nBody: " << resp.body;
  }

  return os;
}
