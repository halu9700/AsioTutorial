#include <fmt/core.h>
#include <asio.hpp>

void print(std::error_code e) {
  fmt::println("Hello, World!");
}

int32_t main(int argc, char* argv[]) {
  asio::io_context io;
  asio::steady_timer t(io, std::chrono::seconds{5});
  t.async_wait(&print);
  print(std::error_code{});
  io.run();
  return 0;
}