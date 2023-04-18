#include "acceptor.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

Acceptor::Acceptor(int port) : port_(port) {
  listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd_ == -1) {
    throw std::runtime_error("Failed to create socket");
  }

  int reuse = 1;
  if (setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) ==
      -1) {
    throw std::runtime_error("Failed to set socket options");
  }

  sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) ==
      -1) {
    throw std::runtime_error("Failed to bind to port " + std::to_string(port));
  }

  if (listen(listen_fd_, SOMAXCONN) == -1) {
    throw std::runtime_error("Failed to listen on port" + std::to_string(port));
  }

  int flags = fcntl(listen_fd_, F_GETFL, 0);
  if (flags == -1) {
    throw std::runtime_error("Failed to get socket flags");
  }

  if (fcntl(listen_fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
    throw std::runtime_error("Failed to set sockt flags");
  }
}

Acceptor::~Acceptor() { close(listen_fd_); }

auto Acceptor::accept(sockaddr_in &client_addr) -> int {
  socklen_t addrlen = sizeof(client_addr);
  int client_fd = ::accept(
      listen_fd_, reinterpret_cast<sockaddr *>(&client_addr), &addrlen);
  if (client_fd == -1) {
    throw std::runtime_error("Failed to accept client connection");
  }
  return client_fd;
}
