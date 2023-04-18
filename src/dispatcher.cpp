#include "dispatcher.h"

#include <algorithm>
#include <utility>

void Dispatcher::addHandler(int fd, EventType event_type,
                            EventHandler handler) {
  event_handlers_[fd][event_type] = std::move(handler);
}

void Dispatcher::removeHandler(int fd, EventType event_type,
                               EventHandler handler) {
  event_handlers_[fd][event_type] = nullptr;
}

void Dispatcher::dispatch(int fd, EventType event_type) {
  auto &handler = event_handlers_[fd][event_type];
  if (handler != nullptr) {
    handler();
  }
}
