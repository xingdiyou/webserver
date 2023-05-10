#include "http_server.h"

#include <sys/epoll.h>

#include "acceptor.h"
#include "reactor.h"

HttpServer::HttpServer(int port) : port_(port) {}

void HttpServer::run() {
  Socket listen_socket;
  listen_socket.bind("0.0.0.0", port_);
  listen_socket.listen();

  Reactor reactor;
  reactor.registerHandler<Acceptor>(listen_socket.getFd(), EPOLLIN | EPOLLET,
                                    listen_socket, reactor);
  reactor.loop();
}
