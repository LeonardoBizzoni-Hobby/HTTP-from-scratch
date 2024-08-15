#include "request.h"

#include <iomanip>

std::ostream &operator<<(std::ostream &os, const http::Request &req) {
  return os << "Request { domain: " << std::quoted(req.domain_name) << ", port: " << req.port
	    << ", query: " << std::quoted(req.query) << ", body: " << std::quoted(req.body) << " }";
}
