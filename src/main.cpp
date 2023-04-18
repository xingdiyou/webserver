#include <error.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

static void process_request(int conn_fd);

auto main(int argc, char* argv[]) -> int {
  // 创建监听socket
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    perror("Failed to create listen socket");
    return -1;
  }

  // 设置socket地址可重用
  int optval = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) <
      0) {
    perror("Failed to set socket options");
    return -1;
  }

  // 绑定地址和端口号
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8080);
  if (bind(listen_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) <
      0) {
    perror("Failed to bind address");
    return -1;
  }

  // 开始监听
  if (listen(listen_fd, 1024) < 0) {
    perror("Failed to listen on socket");
    return -1;
  }

  // 开始接收连接
  std::vector<std::thread> threads;
  while (true) {
    int conn_fd = accept(listen_fd, nullptr, nullptr);
    if (conn_fd < 0) {
      perror("Failed to accept connection");
      break;
    }
    std::thread t(process_request, conn_fd);
    t.detach();
  }

  // 关闭监听socket
  close(listen_fd);
  return 0;
}

static void process_request(int conn_fd) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  int r = read(conn_fd, buffer, sizeof(buffer));
  if (r < 0) {
    perror("Failed to read data from socket");
    close(conn_fd);
    return;
  } else if (r == 0) {
    close(conn_fd);
    return;
  }

  // 发送响应
  const char* response =
      "HTTP/1.1 200 OK\r\nContent-Type: "
      "text/plain\r\nContent-Length: 12\r\n\r\nHello World\n";
  int len = strlen(response);
  int w = write(conn_fd, response, len);
  if (w < len) {
    perror("Failed to write data to socket");
    close(conn_fd);
    return;
  }
}
