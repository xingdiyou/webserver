#pragma once

#include "event_handler.h"

class Event {
 public:
  Event(int fd, int events, EventHandler *handler);

  ~Event();

  auto getFd() -> int;

  auto getEvents() -> int;

  void setEvents(int events);

  auto getHandler() -> EventHandler *;

 private:
  int fd_;
  int events_;
  EventHandler *handler_;
};
