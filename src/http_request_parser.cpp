#include "http_request_parser.h"

#include <cstdlib>
#include <stdexcept>
#include <string_view>

auto HttpRequestParser::parse(std::string_view buffer) -> bool {
  for (const char c : buffer) {
    switch (state_) {
      case ParserState::METHOD: {
        if (c == ' ') {
          state_ = ParserState::URI;
        } else {
          request_.method += c;
        }
        break;
      }
      case ParserState::URI: {
        if (c == ' ') {
          state_ = ParserState::VERSION;
        } else {
          request_.uri += c;
        }
        break;
      }
      case ParserState::VERSION: {
        if (c == '\r') {
          state_ = ParserState::REQUEST_LINE_END;
        } else {
          request_.version += c;
        }
        break;
      }
      case ParserState::REQUEST_LINE_END: {
        if (c != '\n') {
          throw std::runtime_error("Invalid http request");
        }
        state_ = ParserState::HEADER_NAME;
        break;
      }
      case ParserState::HEADER_NAME: {
        if (c == ':') {
          state_ = ParserState::HEADER_SPACE;
        } else if (c == '\r') {
          state_ = ParserState::HEADERS_END;
        } else {
          header_name_ += c;
        }
        break;
      }
      case ParserState::HEADER_SPACE: {
        if (c != ' ') {
          throw std::runtime_error("Invalid http request");
        }
        state_ = ParserState::HEADER_VALUE;
        break;
      }
      case ParserState::HEADER_VALUE: {
        if (c == '\r') {
          state_ = ParserState::HEADER_END;
        } else {
          header_value_ += c;
        }
        break;
      }
      case ParserState::HEADER_END: {
        if (c != '\n') {
          throw std::runtime_error("Invalid http request");
        }
        request_.headers[header_name_] = header_value_;
        header_name_.clear();
        header_value_.clear();
        state_ = ParserState::HEADER_NAME;
        break;
      }
      case ParserState::HEADERS_END: {
        if (c != '\n') {
          throw std::runtime_error("Invalid http request");
        }
        auto it = request_.headers.find("Content-Length");
        if (it != request_.headers.end()) {
          content_length_ = std::atoi(it->second.c_str());
          state_ = ParserState::BODY;
        } else {
          state_ = ParserState::DONE;
        }
        break;
      }
      case ParserState::BODY: {
        request_.body += c;
        if (request_.body.size() == content_length_) {
          state_ = ParserState::DONE;
        }
        break;
      }
      case ParserState::DONE:
        break;
    }
  }
  return state_ == ParserState::DONE;
}

auto HttpRequestParser::getRequest() const -> const HttpRequest& {
  return request_;
}
