#include "epoll_reactor.h"

#include <sys/epoll.h>

#include "fmt/core.h"
#include "glog/logging.h"

EpollReactor::EpollReactor() : epoll_fd_(epoll_create1(0)) {
  if (epoll_fd_ == -1) {
    throw std::runtime_error(
        fmt::format("Failed to create epoll instance: {}", strerror(errno)));
  }
}

EpollReactor::~EpollReactor() { close(epoll_fd_); }

void EpollReactor::registerHandler(int fd, int events,
                                   std::unique_ptr<EventHandler> handler) {
  VLOG(4) << "register handler #" << fd;

  epoll_event event;
  event.data.fd = fd;
  event.events = events;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) == -1) {
    throw std::runtime_error(
        fmt::format("Failed to add event to epoll: {}", strerror(errno)));
  }
  handlers_[fd] = std::move(handler);
};

void EpollReactor::removeHandler(int fd) {
  VLOG(4) << "remove handler #" << fd;

  auto it = handlers_.find(fd);
  if (it == handlers_.end()) {
    LOG(WARNING) << "Failed to find handler #" << fd;
    return;
  }

  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
    throw std::runtime_error(
        fmt::format("Failed to remove event from epoll: {}", strerror(errno)));
  }

  it->second->handleClose();
  handlers_.erase(it);
}

void EpollReactor::loop() {
  while (true) {
    constexpr int kMaxEvents = 1024;
    epoll_event events[kMaxEvents];
    int num_events = epoll_wait(epoll_fd_, events, kMaxEvents, -1);
    if (num_events == -1) {
      throw std::runtime_error(
          fmt::format("Failed to wait events on epoll: {}", strerror(errno)));
    }

    for (int i = 0; i < num_events; i++) {
      int fd = events[i].data.fd;
      auto &handler = handlers_[fd];
      if (events[i].events & EPOLLIN) {
        if (handler->handleRead() < 0) {
          removeHandler(fd);
          continue;
        }
      }
      if (events[i].events & EPOLLOUT) {
        if (handler->handleWrite() < 0) {
          removeHandler(fd);
          continue;
        }
      }
    }
  }
}
