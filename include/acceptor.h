#pragma once

#include <netinet/in.h>

#include <string>

class Acceptor {
 public:
  explicit Acceptor(int port);

  ~Acceptor();

  auto accept(sockaddr_in &client_addr) -> int;

 private:
  int port_;
  int listen_fd_;
};
