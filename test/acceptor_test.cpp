#include "acceptor.h"

#include <gtest/gtest.h>
#include <netinet/in.h>

#include <stdexcept>

TEST(AcceptorTest, TestAcceptor) {
  Acceptor acceptor(8080);
  sockaddr_in client_addr;
  EXPECT_THROW(acceptor.accept(client_addr), std::runtime_error);
}
