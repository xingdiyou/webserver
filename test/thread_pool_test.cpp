#include "thread_pool.h"

#include <gtest/gtest.h>

#include <chrono>
#include <functional>
#include <future>
#include <stdexcept>
#include <thread>

TEST(ThreadPoolTest, AddTask) {
  ThreadPool pool(2);

  std::promise<int> p1;
  std::future<int> f1 = p1.get_future();
  pool.addTask([&p1] { p1.set_value(1); });
  EXPECT_EQ(f1.get(), 1);

  std::promise<int> p2;
  std::future<int> f2 = p2.get_future();
  pool.addTask([&p2](int x, int y) { p2.set_value(x + y); }, 2, 3);
  EXPECT_EQ(f2.get(), 5);
}

TEST(ThreadPoolTest, Stop) {
  ThreadPool pool(2);

  std::promise<void> p1;
  std::future<void> f1 = p1.get_future();
  pool.addTask([&p1] {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p1.set_value();
  });

  pool.stop();
  EXPECT_EQ(f1.wait_for(std::chrono::seconds(2)), std::future_status::timeout);

  EXPECT_THROW(pool.addTask([]() {}), std::runtime_error);
}
