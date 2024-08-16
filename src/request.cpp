#include "request.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ranges>

#include "const_definitions.h"
#include "method.h"

std::ostream &operator<<(std::ostream &os, const http::Request &req) {
  os << "Request { domain: " << std::quoted(req.domain_name) << ", port: " << req.port
     << ", path: " << std::quoted(req.path) << ", query: " << std::quoted(req.query)
     << ", method: " << req.method << ", version: " << (int)req.version.major << "."
     << (int)req.version.minor << ", headers: [ ";

  for (const auto &header : req.optheaders) {
    os << header.first << " = " << header.second << ",";
  }

  os << " ], parameters: [";

  for (const auto &parameter : req.parameters) {
    os << parameter.first << " = " << parameter.second << ",";
  }

  os << " ] }";
  return os;
}

namespace http {
  std::expected<Request, Error> Request::build(std::string_view raw_request) noexcept {
    if (raw_request.empty()) {
      return Err(Error::InvalidRequest);
    }

    Request req;

    auto lines_view = raw_request | std::views::split(NEW_LINE);
    auto lines_view_iter = lines_view.begin();

    auto words = *lines_view_iter++ | std::views::split(' ');
    auto word_iter = words.begin();

    std::string_view method_view(*word_iter++);
    std::string method(method_view);
    std::transform(method_view.cbegin(), method_view.cend(), method.begin(),
		   [](unsigned char ch) { return std::tolower(ch); });

    try {
      req.method = method_map.at(method);
    } catch (...) {
      return Err(Error::InvalidRequestMethod);
    }

    std::string_view query(*word_iter++);
    if (query.empty() || !query.starts_with('/')) {
      return Err(Error::InvalidQueryRequest);
    } else {
      auto query_view = query | std::views::split('?');
      auto query_view_iter = query_view.begin();

      req.path = std::string((*query_view_iter).begin(), (*query_view_iter).end());
      query_view_iter++;

      auto query_param_view =
	  std::string_view((*query_view_iter).begin(), (*query_view_iter).end()) |
	  std::views::split(';');

      auto query_param_view_iter = query_param_view.begin();
      while (query_param_view_iter != query_param_view.end()) {
	auto param_view =
	    std::string_view((*query_param_view_iter).begin(), (*query_param_view_iter).end()) |
	    std::views::split('=');
	auto param_view_iter = param_view.begin();

	auto param_name = std::string((*param_view_iter).begin(), (*param_view_iter).end());
	param_view_iter++;
	req.parameters[std::move(param_name)] = std::string((*param_view_iter).begin(), (*param_view_iter).end());

	++query_param_view_iter;
      }
    }

    std::string_view version(*word_iter);
    auto number = version.substr(version.find_first_of('/') + 1);
    auto major = number.substr(0, number.find_first_of('.'));
    auto minor = number.substr(number.find_first_of('.') + 1);

    try {
      req.version = {.major = (uint8_t)std::stoi(major.data()),
		     .minor = (uint8_t)std::stoi(minor.data())};
    } catch (...) {
      return Err(Error::InvalidHTTPVersion);
    }

    for (std::string_view line; lines_view_iter != lines_view.end() &&
				!(line = std::string_view(*lines_view_iter)).empty();
	 ++lines_view_iter) {
      auto words = line | std::views::split(std::string_view{":"});
      auto it = words.begin();
      std::string field_name((*it).begin(), (*it).end());
      ++it;

      if ((*it)[0] == ' ') {
	req.optheaders[field_name] = std::string((*it).begin() + 1, (*it).end());
      } else {
	req.optheaders[field_name] = std::string((*it).begin(), (*it).end());
      }
    }

    while (++lines_view_iter != lines_view.end()) {
      req.body += std::string((*lines_view_iter).begin(), (*lines_view_iter).end());

      if (std::next(lines_view_iter) != lines_view.end()) {
	req.body += "\n";
      }
    }

    return req;
  }
}  // namespace http
