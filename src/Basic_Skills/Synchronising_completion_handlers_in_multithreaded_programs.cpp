#include <fmt/core.h>
#include <asio.hpp>
#include <functional>

class Printer {
 public:
  Printer(asio::io_context& io)
      : m_strand{asio::make_strand(io)},
        m_timer1{io, asio::chrono::seconds{1}},
        m_timer2{io, asio::chrono::seconds{1}} {
    m_timer1.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::print1, this)));
    m_timer2.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::print2, this)));
  }

  ~Printer() { fmt::println("Final count is {}", m_count); }

  void print1() {
    if (m_count < 10) {
      fmt::println("Timer1 {}", m_count);
      m_count += 1;
      m_timer1.expires_at(m_timer1.expiry() + asio::chrono::seconds{1});
      m_timer1.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::print1, this)));
    }
  }

  void print2() {
    if (m_count < 10) {
      fmt::println("Timer2 {}", m_count);
      m_count += 1;
      m_timer2.expires_at(m_timer2.expiry() + asio::chrono::seconds{1});
      m_timer2.async_wait(asio::bind_executor(m_strand, std::bind(&Printer::print2, this)));
    }
  }

 private:
  asio::strand<asio::io_context::executor_type> m_strand;
  asio::steady_timer m_timer1;
  asio::steady_timer m_timer2;
  int32_t m_count = 0;
};

int32_t main(int argc, char* argv[]) {
  asio::io_context io;
  Printer printer{io};

  // asio::thread t(std::bind(static_cast<std::size_t (asio::io_context::*)()>(&asio::io_context::run), &io));
  std::thread t(static_cast<std::size_t (asio::io_context::*)()>(&asio::io_context::run), &io);
  io.run();
  t.join();
  return 0;
}