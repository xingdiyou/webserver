#include "circular_buffer.h"

#include <algorithm>
#include <cstring>

CircularBuffer::CircularBuffer(int capicity)
    : capacity_(capicity + 1), read_index_(0), write_index_(0) {
  data_ = new char[capacity_];
}

CircularBuffer::~CircularBuffer() { delete[] data_; }

auto CircularBuffer::readable() -> int {
  return (write_index_ + capacity_ - read_index_) % capacity_;
}

auto CircularBuffer::writable() -> int { return capacity_ - readable() - 1; }

auto CircularBuffer::read(char *data, int len) -> int {
  int n = std::min(len, readable());
  for (int i = 0; i < n; i++) {
    data[i] = data_[read_index_];
    read_index_ = (read_index_ + 1) % capacity_;
  }
  return n;
}

auto CircularBuffer::write(const char *data, int len) -> int {
  int n = std::min(len, writable());
  for (int i = 0; i < n; i++) {
    data_[write_index_] = data[i];
    write_index_ = (write_index_ + 1) % capacity_;
  }
  return n;
}
