#pragma once

#include "event_handler.h"

class HttpRequestHandler : public EventHandler {
 public:
  void handleRead() override;

  void handleWrite() override;

  void handleClose() override;

  void handleError() override;
};
