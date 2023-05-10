#include "http_server.h"

#include <sys/epoll.h>

#include <memory>

#include "acceptor.h"
#include "epoll_reactor.h"

HttpServer::HttpServer(int port) : port_(port) {}

void HttpServer::run() {
  Socket listen_socket;
  listen_socket.bind("0.0.0.0", port_);
  listen_socket.listen();

  EpollReactor reactor;
  reactor.registerHandler(listen_socket.getFd(), EPOLLIN | EPOLLET,
                          std::make_unique<Acceptor>(listen_socket, reactor));
  reactor.loop();
}
