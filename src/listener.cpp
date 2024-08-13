#include "listener.h"

namespace http {
  std::expected<Listener, Error> Listener::create_on_local(uint16_t port) {
    Listener server;

    ++port;

    return server;
  }


  void Listener::serve() {

  }
}  // namespace http
