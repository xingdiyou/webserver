#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "http_request.h"

class HttpRequestParser {
 public:
  HttpRequestParser() : state_(ParserState::METHOD) {}

  auto parse(std::string_view buffer) -> bool;

  auto getRequest() const -> const HttpRequest&;

 private:
  enum class ParserState {
    METHOD,
    URI,
    VERSION,
    REQUEST_LINE_END,
    HEADER_NAME,
    HEADER_SPACE,
    HEADER_VALUE,
    HEADER_END,
    HEADERS_END,
    BODY,
    DONE
  };

  ParserState state_;
  std::string header_name_;
  std::string header_value_;
  size_t content_length_;
  HttpRequest request_;
};
