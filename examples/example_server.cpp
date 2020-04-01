//
// Created by kira on 4/1/20.
//

#include "tcp_server.h"
#include "global_logger.h"

int main() {
    tcp::Server server("0.0.0.0", 8080);
    server.set_max_connections(16);
    try {
        while (server.is_opened()) {
            auto connection = server.accept();
            log::warn("dst_addr: " + connection->dst_addr());
            log::warn("dst_port: " + std::to_string(connection->dst_port()));
            log::warn("src_addr: " + connection->src_addr());
            log::warn("src_port: " + std::to_string(connection->src_port()));

            std::string to_read(1024, 0);
            to_read.resize(connection->read(to_read.data(), to_read.size()));

            log::info(to_read);

            std::string to_write("MUDAMUDAMUDA");
            connection->write(to_write.c_str(), to_write.size());
        }
    } catch (process::Exception &e) {
        log::create_stderr_logger()->error(e.what());
        server.close();
    }
    return 0;
}
