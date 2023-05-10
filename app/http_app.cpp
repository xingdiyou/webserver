#include <glog/logging.h>

#include <exception>

#include "http_server.h"

auto main(int argc, char **argv) -> int {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = true;
  FLAGS_v = 4;

  HttpServer server(8080);
  server.run();
}
