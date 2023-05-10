#pragma once

#include <sys/epoll.h>

#include <memory>
#include <unordered_map>
#include <utility>

#include "event_handler.h"
#include "fmt/core.h"
#include "glog/logging.h"

class Reactor {
 public:
  Reactor();

  ~Reactor();

  template <typename Handler, typename... Args>
  void registerHandler(int fd, int events, Args &&...args) {
    static_assert(std::is_base_of_v<EventHandler, Handler>);

    VLOG(4) << "register handler #" << fd;

    epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) == -1) {
      throw std::runtime_error(
          fmt::format("Failed to add event to epoll: {}", strerror(errno)));
    }
    handlers_[fd] = std::make_unique<Handler>(args...);
  };

  void removeHandler(int fd);

  void loop();

 private:
  int epoll_fd_;
  std::unordered_map<int, std::unique_ptr<EventHandler>> handlers_;
};
