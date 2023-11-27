#include <fmt/core.h>
#include <asio.hpp>

int32_t main(int argc, char* argv[]) {
  asio::io_context io;
  asio::steady_timer t(io, std::chrono::seconds{5});
  t.wait();
  fmt::println("Hello, World!\n");
  return 0;
}