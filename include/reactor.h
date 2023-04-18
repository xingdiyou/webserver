#pragma once

#include "event_handler.h"

class Reactor {
 public:
  virtual ~Reactor();

  virtual void addHandler(int fd, int events, EventHandler* handler) = 0;

  virtual void removeHandler(int fd) = 0;

  virtual void run() = 0;
};
