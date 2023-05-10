#include "http_connection.h"

#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <glog/logging.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string_view>

#include "http_request.h"
#include "http_response.h"
#include "socket.h"

HttpConnection::HttpConnection(Socket client_socket, Reactor &reactor)
    : client_socket_(client_socket), reactor_(reactor) {}

auto HttpConnection::handleRead() -> int {
  char buf[1024];
  while (true) {
    ssize_t n = client_socket_.read(buf, sizeof(buf));
    if (n == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      }
      LOG(ERROR) << "Failed to read from socket: " << strerror(errno);
      return -1;
    }
    if (n == 0) {
      VLOG(5) << "Connection closed by peer";
      return -1;
    }
    request_parser_.parse(std::string_view(buf, n));
  }

  auto &request = request_parser_.getRequest();

  VLOG(5) << request.toString();

  HttpResponse http_response;
  http_response.version = "HTTP/1.1";
  http_response.statusCode = "200";
  http_response.statusText = "OK";
  http_response.headers.emplace_back("Content-Length", "12");
  http_response.body = "Hello World!";
  std::string response = http_response.toString();
  client_socket_.write(response.data(), response.size());
  client_socket_.shutdown(SHUT_WR);

  return -1;
}

auto HttpConnection::handleWrite() -> int {
  LOG(ERROR) << "Unexpected write event";
  return 0;
}

void HttpConnection::handleClose() { client_socket_.close(); }
