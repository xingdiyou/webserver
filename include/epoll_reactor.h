#pragma once

#include <unordered_map>
#include <vector>

#include "event.h"
#include "reactor.h"

class EpollReactor : public Reactor {
 public:
  void addHandler(int fd, int events, EventHandler* handler) override;

  void removeHandler(int fd) override;

  void run() override;

 private:
  int epoll_fd_;
  std::unordered_map<int, Event> events_;
};
