#pragma once

#include "buffer.h"

class CircularBuffer : public Buffer {
 public:
  explicit CircularBuffer(int size = 1024);

  ~CircularBuffer() override;

  auto readable() -> int override;

  auto writable() -> int override;

  auto read(char *data, int len) -> int override;

  auto write(const char *data, int len) -> int override;

 private:
  int capacity_;
  int read_index_;
  int write_index_;
  char *data_;
};
