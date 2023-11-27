#include <fmt/core.h>
#include <asio.hpp>

class Printer {
 public:
  Printer(asio::io_context& io) : m_timer(io, std::chrono::seconds{1}), m_count{0} {
    m_timer.async_wait(std::bind(&Printer::print, this));
  }

  ~Printer() { fmt::println("Final count is {}", m_count); }

  void print() {
    if (m_count < 5) {
      m_count += 1;
      fmt::println("{}", m_count);
      m_timer.expires_at(m_timer.expiry() + asio::chrono::seconds{1});
      m_timer.async_wait(std::bind(&Printer::print, this));
    }
  }

 private:
  asio::steady_timer m_timer;
  int m_count;
};

int32_t main(int argc, char* argv[]) {
  asio::io_context io;
  Printer printer(io);
  io.run();
  return 0;
}