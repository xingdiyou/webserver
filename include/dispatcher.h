#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

enum class EventType { EVENT_READ, EVENT_WRITE, EVENT_ERROR };

using EventHandler = std::function<void()>;

class Dispatcher {
 public:
  void addHandler(int fd, EventType event_type, EventHandler handler);

  void removeHandler(int fd, EventType event_type, EventHandler handler);

  void dispatch(int fd, EventType event_type);

 private:
  std::unordered_map<int, std::unordered_map<EventType, EventHandler>>
      event_handlers_;
};
