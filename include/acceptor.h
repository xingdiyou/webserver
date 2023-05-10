#pragma once

#include "event_handler.h"
#include "reactor.h"
#include "socket.h"

class Acceptor : public EventHandler {
 public:
  Acceptor(Socket listen_socket, Reactor &reactor);

  auto handleRead() -> int override;

  void handleClose() override;

 private:
  Socket listen_socket_;
  Reactor &reactor_;
};
