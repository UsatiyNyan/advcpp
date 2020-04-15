//
// Created by kira on 4/15/20.
//

#include "epoll_connection.h"
#include <iostream>

int main() {
    epoll::Connection connection("0.0.0.0", 8080);
    try {
        std::string to_write("ORAORAORA");
        connection.write(to_write.c_str(), to_write.length());
        connection.await_write();

        std::string to_read(12, 0);
        connection.read(to_read.length());
        connection.await_read(to_read.data());
        std::cout << to_read << std::endl;

    } catch (std::exception & e) {
        e.what();
    }

    return 0;
}

