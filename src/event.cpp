#include "event.h"

Event::Event(int fd, int events, EventHandler *handler)
    : fd_(fd), events_(events), handler_(handler) {}

auto Event::getFd() -> int { return fd_; }

auto Event::getEvents() -> int { return events_; }

void Event::setEvents(int events) { events_ = events; }

auto Event::getHandler() -> EventHandler * { return handler_; }
