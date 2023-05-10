#pragma once

class HttpServer {
 public:
  explicit HttpServer(int port);

  void run();

 private:
  int port_;
};
