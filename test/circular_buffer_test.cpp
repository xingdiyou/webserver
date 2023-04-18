#include "circular_buffer.h"

#include <gtest/gtest.h>

#include <random>
#include <string>
#include <string_view>

std::string randomString(int length) {
  std::string str(length, 0);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist('A', 'Z');
  for (size_t i = 0; i < length; ++i) {
    str[i] = dist(gen);
  }
  return str;
}

TEST(CircularBufferTest, TestCircularBuffer) {
  CircularBuffer buffer(100);
  EXPECT_EQ(buffer.writable(), 100);

  for (int i = 0; i < 100; i++) {
    std::string str1 = randomString(200);
    EXPECT_EQ(buffer.write(str1.c_str(), 200), 100);
    EXPECT_EQ(buffer.writable(), 0);
    EXPECT_EQ(buffer.readable(), 100);

    std::string str2(50, 0);
    EXPECT_EQ(buffer.read(str2.data(), 50), 50);
    EXPECT_EQ(buffer.writable(), 50);
    EXPECT_EQ(buffer.readable(), 50);
    EXPECT_EQ(std::string_view(str1.c_str(), 50),
              std::string_view(str2.c_str(), 50));

    EXPECT_EQ(buffer.read(str2.data(), 50), 50);
    EXPECT_EQ(buffer.writable(), 100);
    EXPECT_EQ(buffer.readable(), 0);
    EXPECT_EQ(std::string_view(str1.c_str() + 50, 50),
              std::string_view(str2.c_str(), 50));
  }
}
