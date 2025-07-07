#include <gtest/gtest.h>
#include <stdexec/execution.hpp>

#include <iomisc/dummy.hpp>

TEST(Tmp, Tmp) {
  stdexec::sender auto future = stdexec::just(3) | stdexec::then([](int x) {
                                  return x + 1;
                                });

  auto [x] = stdexec::sync_wait(future).value();
  ASSERT_EQ(x, 4);

  iomisc::hello_world();
}
