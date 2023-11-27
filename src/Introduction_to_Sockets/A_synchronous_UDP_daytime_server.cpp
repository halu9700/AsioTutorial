#include <fmt/core.h>
#include <asio.hpp>
#include <ctime>

using namespace asio::ip;

std::string make_daytime_string() {
  using namespace std;   // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int32_t main(int argc, char* argv[]) {
  try {
    asio::io_context io_context;
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));
    for (;;) {
      std::array<char, 1> recv_buf;
      udp::endpoint remote_endpoint;
      socket.receive_from(asio::buffer(recv_buf), remote_endpoint);
      std::string message = make_daytime_string();
      asio::error_code ec;
      socket.send_to(asio::buffer(message), remote_endpoint, 0, ec);
    }
  } catch (std::exception& e) {
    fmt::println(e.what());
  }

  return 0;
}