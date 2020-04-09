//
// Created by kira on 4/9/20.
//

#include "tcp_server.h"
#include <iostream>


int main() {
    tcp::Server server("0.0.0.0", 8080);
    server.set_max_connections(1);
    try {
        while (server.is_opened()) {
            auto connection = server.accept();
            std::cout << "dst_addr: " << connection.dst_addr() << std::endl;
            std::cout << "dst_port: " << connection.dst_port() << std::endl;
            std::cout << "src_addr: " << connection.src_addr() << std::endl;
            std::cout << "src_port: " << connection.src_port() << std::endl;

            std::string to_read(1024, 0);
            to_read.resize(connection.read(to_read.data(), to_read.length()));
            std::cout << to_read << std::endl;

            std::string to_write("MUDAMUDAMUDA");
            connection.write(to_write.c_str(), to_write.length());
        }
    } catch (std::exception & e) {
        std::cerr << e.what();
    }
    return 0;
}
