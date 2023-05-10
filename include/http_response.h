#pragma once

#include <string>
#include <vector>

struct HttpResponse {
  std::string version;
  std::string statusCode;
  std::string statusText;
  std::vector<std::pair<std::string, std::string>> headers;
  std::string body;

  auto toString() -> std::string;
};
