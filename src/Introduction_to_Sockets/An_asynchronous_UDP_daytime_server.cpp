#include <fmt/core.h>
#include <asio.hpp>
#include <functional>
using namespace asio::ip;

std::string make_daytime_string() {
  using namespace std;   // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class udp_server {
 public:
  udp_server(asio::io_context& io_context) : m_socket(io_context, udp::endpoint(udp::v4(), 13)) { start_service(); }

 private:
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

  void handle_send() {}

  udp::socket m_socket;
  udp::endpoint m_remote_endpoint;
  std::array<char, 1> m_recv_buf;
};

int32_t main(int argc, char* argv[]) {
  try {
    asio::io_context io_context;
    udp_server server(io_context);
    io_context.run();
  } catch (std::exception e) {
    fmt::println(e.what());
  }

  return 0;
}