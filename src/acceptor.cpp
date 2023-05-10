#include "acceptor.h"

#include <sys/epoll.h>

#include <cerrno>
#include <cstring>
#include <memory>

#include "glog/logging.h"
#include "http_connection.h"

Acceptor::Acceptor(Socket listen_socket, Reactor &reactor)
    : listen_socket_(listen_socket), reactor_(reactor) {}

auto Acceptor::handleRead() -> int {
  while (true) {
    sockaddr_in client_addr{};
    int client_fd = listen_socket_.accept(client_addr);
    if (client_fd == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      LOG(ERROR) << "Failed to accept connection: " << strerror(errno);
      return -1;
    }
    reactor_.registerHandler(
        client_fd, EPOLLIN | EPOLLET,
        std::make_unique<HttpConnection>(Socket(client_fd), reactor_));
  }
  return 0;
}

void Acceptor::handleClose() { listen_socket_.close(); }
