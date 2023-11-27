#include <fmt/core.h>
#include <asio.hpp>
#include <functional>

void print(std::error_code e, asio::steady_timer& t, int& count) {
  if (count < 5) {
    fmt::println("{}", count);
    count += 1;
    t.expires_at(t.expiry() + std::chrono::seconds{1});
    t.async_wait(std::bind(print, std::placeholders::_1, std::ref(t), count));
  }
}

int32_t main(int argc, char* argv[]) {
  asio::io_context io;
  asio::steady_timer t(io, std::chrono::seconds{1});
  int count = 0;
  t.async_wait(std::bind(print, std::placeholders::_1, std::ref(t), count));
  io.run();
  return 0;
}