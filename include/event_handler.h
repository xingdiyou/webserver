#pragma once

class EventHandler {
 public:
  virtual ~EventHandler();

  virtual auto handleRead() -> int;

  virtual auto handleWrite() -> int;

  virtual void handleClose();
};
