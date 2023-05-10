#pragma once

#include <memory>
#include <unordered_map>

#include "event_handler.h"
#include "reactor.h"

class EpollReactor : public Reactor {
 public:
  EpollReactor();

  ~EpollReactor() override;

  void registerHandler(int fd, int events,
                       std::unique_ptr<EventHandler> handler) override;

  void removeHandler(int fd) override;

  void loop() override;

 private:
  int epoll_fd_;
  std::unordered_map<int, std::unique_ptr<EventHandler>> handlers_;
};
