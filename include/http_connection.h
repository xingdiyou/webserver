#pragma once

#include "event_handler.h"
#include "http_request_parser.h"
#include "reactor.h"
#include "socket.h"

class HttpConnection : public EventHandler {
 public:
  explicit HttpConnection(Socket client_socket, Reactor &reactor);

  auto handleRead() -> int override;

  auto handleWrite() -> int override;

  void handleClose() override;

 private:
  Socket client_socket_;
  Reactor &reactor_;
  HttpRequestParser request_parser_;
};
