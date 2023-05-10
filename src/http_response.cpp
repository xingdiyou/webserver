#include "http_response.h"

#include <sstream>

auto HttpResponse::toString() -> std::string {
  std::stringstream ss;
  ss << version << " " << statusCode << " " << statusText << "\r\n";
  ss << "Content-Length: " << body.size() << "\r\n";
  ss << "\r\n";
  ss << body;
  return ss.str();
}
