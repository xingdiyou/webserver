#include "socket.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>

#include "fmt/core.h"

Socket::Socket() : fd_(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) {
  if (fd_ == -1) {
    throw std::runtime_error(
        fmt::format("Failed to create socket: {}", strerror(errno)));
  }
}

Socket::Socket(int fd) : fd_(fd) {}

void Socket::bind(const sockaddr_in &addr) {
  int optval = 1;
  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
      -1) {
    throw std::runtime_error(
        fmt::format("Failed to set socket options: {}", strerror(errno)));
  }

  if (::bind(fd_, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) ==
      -1) {
    throw std::runtime_error(
        fmt::format("Failed to bind socket: {}", strerror(errno)));
  }
}

void Socket::bind(std::string_view ip, int port) {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.data(), &addr.sin_addr) != 1) {
    throw std::runtime_error(
        fmt::format("Invalid ip address: {}", strerror(errno)));
  }
  bind(addr);
}

void Socket::listen(int backlog) {
  if (::listen(fd_, backlog) == -1) {
    throw std::runtime_error(
        fmt::format("Failed to listen on socket: {}", strerror(errno)));
  }
}

auto Socket::accept(sockaddr_in &addr) -> int {
  socklen_t addrlen = sizeof(addr);
  return accept4(fd_, reinterpret_cast<sockaddr *>(&addr), &addrlen,
                 SOCK_NONBLOCK);
}

auto Socket::connect(const sockaddr_in &addr) -> int {
  return ::connect(fd_, reinterpret_cast<const sockaddr *>(&addr),
                   sizeof(addr));
}

void Socket::close() {
  if (fd_ != -1) {
    ::close(fd_);
    fd_ = -1;
  }
}

void Socket::shutdown(int how) { ::shutdown(fd_, how); }

auto Socket::read(void *buf, size_t count) -> ssize_t {
  return ::read(fd_, buf, count);
}

auto Socket::write(const void *buf, size_t count) -> ssize_t {
  return ::write(fd_, buf, count);
}

auto Socket::getFd() const -> int { return fd_; }
