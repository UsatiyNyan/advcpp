//
// Created by kira on 4/1/20.
//

#include "tcp_connection.h"
#include "global_logger.h"

int main() {
    tcp::Connection connection("192.168.0.110", 8080);
    connection.set_timeout(10);

    log::warn("dst_addr: " + connection.dst_addr());
    log::warn("dst_port: " + std::to_string(connection.dst_port()));
    log::warn("src_addr: " + connection.src_addr());
    log::warn("src_port: " + std::to_string(connection.src_port()));

    std::string to_write("ORAORAORA");
    connection.write(to_write.c_str(), to_write.size());

    std::string to_read(1024, 0);
    to_read.resize(connection.read(to_read.data(), to_read.size()));

    log::info(to_read);
    return 0;
}