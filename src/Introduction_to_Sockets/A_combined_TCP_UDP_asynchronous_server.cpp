#include <fmt/core.h>
#include <asio.hpp>

using namespace asio::ip;

std::string make_daytime_string() {
  using namespace std;   // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
 public:
  using pointer = std::shared_ptr<tcp_connection>;

  static pointer create(asio::io_context& io_context) { return std::make_shared<tcp_connection>(io_context); }

  tcp::socket& socket() { return m_socket; }

  void start() {
    m_message = make_daytime_string();
    asio::async_write(m_socket, asio::buffer(m_message), std::bind(&tcp_connection::handle_write, shared_from_this()));
  }

  void handle_write() {}

  tcp_connection(asio::io_context& io_context) : m_socket(io_context) {}

  tcp::socket m_socket;
  std::string m_message;
};

class tcp_server {
 public:
  tcp_server(asio::io_context& io_context)
      : m_io_context(io_context), m_acceptor(io_context, tcp::endpoint(tcp::v4(), 13)) {
    start_accept();
  }

  void start_accept() {
    tcp_connection::pointer connection = tcp_connection::create(m_io_context);

    m_acceptor.async_accept(connection->socket(),
                            std::bind(&tcp_server::handle_accept, this, std::placeholders::_1, connection));
  }

  void handle_accept(asio::error_code ec, tcp_connection::pointer conn) {
    if (!ec) {
      conn->start();
    }
    start_accept();
  }

  tcp::acceptor m_acceptor;
  asio::io_context& m_io_context;
};

class udp_server {
 public:
  udp_server(asio::io_context& io_context) : m_socket(io_context, udp::endpoint(udp::v4(), 13)) { start_service(); }

  void start_service() {
    m_socket.async_receive_from(asio::buffer(m_recv_buf), m_remote_endpoint,
                                std::bind(&udp_server::handle_recieve, this, std::placeholders::_1));
  }

  void handle_recieve(asio::error_code ec) {
    if (!ec) {
      auto message = std::make_shared<std::string>(make_daytime_string());
      m_socket.async_send_to(asio::buffer(*message), m_remote_endpoint, std::bind(&udp_server::start_service, this));
    }
  }

  udp::socket m_socket;
  udp::endpoint m_remote_endpoint;
  std::array<char, 1> m_recv_buf;
};

int32_t main(int argc, char* argv[]) {
  try {
    asio::io_context io_context;
    tcp_server tcps(io_context);
    udp_server udps(io_context);
    io_context.run();
  } catch (std::exception e) {
    fmt::println(e.what());
  }
  return 0;
}