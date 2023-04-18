#pragma once

class EventHandler {
 public:
  virtual ~EventHandler();

  virtual void handleRead() = 0;

  virtual void handleWrite() = 0;

  virtual void handleClose() = 0;

  virtual void handleError() = 0;
};
