#include "request.h"

#include <iomanip>

std::ostream &operator<<(std::ostream &os, const http::Request &req) {
  return os << "Request { domain: " << std::quoted(req.domain_name) << ", port: " << req.port
	    << ", host: " << std::quoted(req.host) << ", query: " << std::quoted(req.query)
	    << ", accept: " << std::quoted(req.accept) << ", body: " << std::quoted(req.body)
	    << " }";
}
