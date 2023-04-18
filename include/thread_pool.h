#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <semaphore>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(int thread_count);

  ~ThreadPool();

  template <typename Function, typename... Args>
  void addTask(Function &&function, Args &&...args) {
    std::scoped_lock lk(mutex_);
    if (!running_) {
      throw std::runtime_error("Thread pool has stopped");
    }

    auto task = std::bind(std::forward<Function>(function),
                          std::forward<Args>(args)...);
    tasks_.push_back(std::move(task));
    cv_.notify_all();
  }

  void stop();

 private:
  std::vector<std::thread> threads_;
  std::deque<std::function<void()>> tasks_;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool running_;
};
