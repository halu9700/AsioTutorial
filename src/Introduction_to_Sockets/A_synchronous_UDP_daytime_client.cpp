#include <fmt/core.h>
#include <asio.hpp>

using namespace asio::ip;

int32_t main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      fmt::println("Usage: client <host>");
      return 1;
    }
    asio::io_context io_context;
    udp::resolver resolver(io_context);
    udp::endpoint reciver_endpoint = *resolver.resolve(udp::v4(), argv[1], "daytime").begin();
    udp::socket socket(io_context);
    socket.open(udp::v4());
    std::array<char, 1> send_buf = {{0}};
    socket.send_to(asio::buffer(send_buf), reciver_endpoint);

    std::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
    fmt::println(std::string(recv_buf.begin(), recv_buf.begin() + len));

  } catch (std::exception& e) {
    fmt::println(e.what());
  }
  return 0;
}