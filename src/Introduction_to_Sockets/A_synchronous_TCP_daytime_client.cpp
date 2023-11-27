#include <fmt/core.h>
#include <fmt/ranges.h>
#include <asio.hpp>
#include <iostream>

int32_t main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      fmt::println("Usage: client <host>");
      return 1;
    }
    asio::io_context io;
    asio::ip::tcp::resolver resolver(io);
    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
    asio::ip::tcp::socket socket(io);
    asio::connect(socket, endpoints);
    for (;;) {
      std::array<char, 128> buf;
      asio::error_code error;
      size_t len = socket.read_some(asio::buffer(buf), error);
      if (error == asio::error::eof) {
        break;
      } else if (error) {
        throw asio::system_error(error);
      }
      fmt::println(fmt::format("{}", std::string(buf.begin(), buf.begin() + len)));
    }
  } catch (std::exception& e) {
    fmt::println(e.what());
  }
  return 0;
}