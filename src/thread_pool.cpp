#include "thread_pool.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <utility>

ThreadPool::ThreadPool(int num_threads) : running_(true) {
  for (int i = 0; i < num_threads; i++) {
    threads_.emplace_back([this] { workerThread(); });
  }
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::stop() {
  {
    std::scoped_lock lk(mutex_);
    if (!running_) {
      cv_.notify_all();
      return;
    }

    running_ = false;
    tasks_.clear();
    cv_.notify_all();
  }

  for (auto &thread : threads_) {
    thread.join();
  }
}

void ThreadPool::workerThread() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock lk(mutex_);
      cv_.wait(lk, [this]() { return !running_ || !tasks_.empty(); });
      if (!running_) {
        cv_.notify_all();
        break;
      }

      task = std::move(tasks_.front());
      tasks_.pop_front();
    }
    task();
  }
}
