#include "http_request_parser.h"

#include <gtest/gtest.h>

TEST(HttpRequestParserTest, RequestWithoutBody) {
  const std::string request_string =
      "GET /hello/world HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "\r\n";
  HttpRequestParser parser;
  ASSERT_TRUE(parser.parse(request_string));
  const HttpRequest& request = parser.getRequest();
  ASSERT_EQ(request.method, "GET");
  ASSERT_EQ(request.uri, "/hello/world");
  ASSERT_EQ(request.version, "HTTP/1.1");
  ASSERT_EQ(request.headers.size(), 1);
  ASSERT_EQ(request.headers.at("Host"), "example.com");
  ASSERT_TRUE(request.body.empty());
}

TEST(HttpRequestParserTest, RequestWithBody) {
  const std::string request_string =
      "POST /submit HTTP/1.0\r\n"
      "Host: example.com\r\n"
      "Content-Length: 13\r\n"
      "\r\n"
      "Hello, world!";
  HttpRequestParser parser;
  ASSERT_TRUE(parser.parse(request_string));
  const HttpRequest& request = parser.getRequest();
  ASSERT_EQ(request.method, "POST");
  ASSERT_EQ(request.uri, "/submit");
  ASSERT_EQ(request.version, "HTTP/1.0");
  ASSERT_EQ(request.headers.size(), 2);
  ASSERT_EQ(request.headers.at("Host"), "example.com");
  ASSERT_EQ(request.headers.at("Content-Length"), "13");
  ASSERT_EQ(request.body, "Hello, world!");
}

TEST(HttpRequestParserTest, RequestWithoutHeaders) {
  HttpRequestParser parser;
  std::string_view buffer =
      "GET / HTTP/1.1\r\n"
      "\r\n";

  ASSERT_TRUE(parser.parse(buffer));
  const auto& request = parser.getRequest();
  EXPECT_EQ(request.method, "GET");
  EXPECT_EQ(request.uri, "/");
  EXPECT_EQ(request.version, "HTTP/1.1");
  EXPECT_EQ(request.headers.size(), 0);
  EXPECT_EQ(request.body, "");
}

TEST(HttpRequestParserTest, InvalidRequest) {
  const std::string request_string =
      "GET /hello/world HTTP/1.1\r"
      "Host: example.com\r\n"
      "Invalid-Header: value\r\n";
  HttpRequestParser parser;
  ASSERT_THROW(parser.parse(request_string), std::runtime_error);
}
