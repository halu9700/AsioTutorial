#include <fmt/core.h>
#include <asio.hpp>

using namespace asio::ip;

std::string make_daytime_string() {
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
 public:
  using pointer = std::shared_ptr<tcp_connection>;

  static pointer create(asio::io_context& io_context) { return pointer(new tcp_connection(io_context)); }

  tcp::socket& socket() { return m_socket; }

  void start() {
    m_message = make_daytime_string();
    asio::async_write(m_socket, asio::buffer(m_message), std::bind(&tcp_connection::handle_write, shared_from_this()));
  }

 private:
  tcp_connection(asio::io_context& io_context) : m_socket(io_context) {}

  void handle_write() {}

  tcp::socket m_socket;
  std::string m_message;
};

class tcp_server {
 public:
  tcp_server(asio::io_context& io_context)
      : m_io_context(io_context), m_acceptor(io_context, tcp::endpoint(tcp::v4(), 13)) {
    start_accept();
  }

 private:
  void start_accept() {
    tcp_connection::pointer new_connection = tcp_connection::create(m_io_context);
    m_acceptor.async_accept(new_connection->socket(),
                            std::bind(&tcp_server::handle_accept, this, new_connection, std::placeholders::_1));
  }

  void handle_accept(tcp_connection::pointer new_connection, asio::error_code const& error) {
    if (!error) {
      new_connection->start();
    }
    start_accept();
  }

  asio::io_context& m_io_context;
  tcp::acceptor m_acceptor;
};

int32_t main(int argc, char* argv[]) {
  try {
    asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  } catch (std::exception& e) {
    fmt::println(e.what());
  }

  return 0;
}