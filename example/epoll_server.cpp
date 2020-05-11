//
// Created by kira on 4/15/20.
//

#include "epoll_server.h"

#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main() {
    auto on_read = [](epoll::Connection &conn) {
      std::string to_read(9, 0);
      static int i = 1;
      if (i == 1) {
          conn.read(to_read.length());
          std::cout << "reading on " << conn.dst_addr() << std::endl;
          i--;
      }
      conn.try_read();
      if (!conn.is_readable()) {
          conn.finish_read(to_read.data());
          std::cout << to_read << std::endl;
      }
    };

    auto on_write = [](epoll::Connection &conn) {
      std::string to_write("MUDAMUDAMUDA");
      static int i = 1;
      if (i == 1) {
          conn.write(to_write.c_str(), to_write.length());
          i--;
      }
      static auto time = std::chrono::system_clock::now() + 10s;
      if (std::chrono::system_clock::now() >= time) {
          std::cout << "writing to " << conn.dst_addr() << std::endl;
          conn.try_write();
      }
    };

    epoll::Server server("0.0.0.0", 8080, on_read, on_write);
    server.spin();
    return 0;
}
