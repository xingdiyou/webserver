#include "http_request.h"

#include <sstream>

auto HttpRequest::toString() const -> std::string {
  std::stringstream ss;
  ss << method << " " << uri << " " << version << "\r\n";
  for (const auto& [key, value] : headers) {
    ss << key << ": " << value << "\r\n";
  }
  if (!body.empty()) {
    ss << "\r\n" << body;
  }
  return ss.str();
}
