#pragma once

#include <string>

#include "event_handler.h"

class Handler : public EventHandler {
 public:
  Handler();

  void handleRead() override;

  void handleWrite() override;

  void handleClose() override;

  void handleError() override;

 private:
  int socket_fd_;
  std::string buffer_;
};
