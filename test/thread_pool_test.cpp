#include "thread_pool.h"

#include <gtest/gtest.h>

#include <chrono>
#include <functional>
#include <stdexcept>
#include <thread>

TEST(ThreadPoolTest, TestThreadPool) {
  ThreadPool pool(4);

  std::atomic<int> result = 0;
  for (int i = 1; i <= 100; i++) {
    pool.addTask(
        [](std::atomic<int> &a, int b) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          a += b;
        },
        std::ref(result), i);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  EXPECT_EQ(result, 5050);

  pool.stop();
  EXPECT_THROW(pool.addTask([]() {}), std::runtime_error);
}
