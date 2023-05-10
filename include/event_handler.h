#pragma once

class EventHandler {
 public:
  virtual ~EventHandler() = default;

  virtual auto handleRead() -> int;

  virtual auto handleWrite() -> int;

  virtual void handleClose() = 0;
};
