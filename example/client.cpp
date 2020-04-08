//
// Created by kira on 4/9/20.
//

#include "tcp_connection.h"
#include <iostream>

int main() {
    tcp::Connection connection("0.0.0.0", 8080);
    try {
        std::string to_write("ORAORAORA");
        connection.write(to_write.c_str(), to_write.length());

        std::string to_read(1024, 0);
        to_read.resize(connection.read(to_read.data(), to_read.length()));
        std::cout << to_read << std::endl;

    } catch (std::exception & e) {
        e.what();
    }

    return 0;
}
