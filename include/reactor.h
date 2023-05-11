#pragma once

#include <memory>

#include "event_handler.h"

class Reactor {
 public:
  virtual ~Reactor();

  virtual void registerHandler(int fd, int events,
                               std::unique_ptr<EventHandler> handler) = 0;

  virtual void removeHandler(int fd) = 0;

  virtual void loop() = 0;
};
