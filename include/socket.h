#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string_view>

class Socket {
 public:
  Socket();

  explicit Socket(int fd);

  void bind(const sockaddr_in& addr);

  void bind(const std::string_view ip, int port);

  void listen(int backlog = SOMAXCONN);

  auto accept(sockaddr_in& addr) -> int;

  auto connect(const sockaddr_in& addr) -> int;

  void close();

  void shutdown(int how);

  auto read(void* buf, size_t count) -> ssize_t;

  auto write(const void* buf, size_t count) -> ssize_t;

  [[nodiscard]] auto getFd() const -> int;

 private:
  int fd_;
};
