#pragma once

class Buffer {
 public:
  virtual ~Buffer() = default;

  virtual auto readable() -> int = 0;

  virtual auto writable() -> int = 0;

  virtual auto read(char *data, int len) -> int = 0;

  virtual auto write(const char *data, int len) -> int = 0;
};
