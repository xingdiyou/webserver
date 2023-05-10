#pragma once

#include <string>
#include <unordered_map>

struct HttpRequest {
  std::string method;
  std::string uri;
  std::string version;
  std::string body;
  std::unordered_map<std::string, std::string> headers;

  auto toString() const -> std::string;
};
