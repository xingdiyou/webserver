#include "epoll_reactor.h"

#include "event.h"

void EpollReactor::registerHandler(int fd, int events, EventHandler* handler) {
  events_[fd] = Event(fd, events, handler);
}

void EpollReactor::unregisterHandler(int fd) { events_.erase(fd); }

void EpollReactor::run() {}
