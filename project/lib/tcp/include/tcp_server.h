//
// Created by kira on 3/31/20.
//

#ifndef PROJECT_LIB_TCP_SERVER_H_
#define PROJECT_LIB_TCP_SERVER_H_

#include "file_descriptor.h"
#include "tcp_connection.h"
#include <memory>

namespace tcp {
class Server {
 public:
    Server(std::string ip, uint16_t port);
    ~Server();
    Connection accept();

    void open(std::string ip, uint16_t port);
    void close();

    void set_max_connections(int max_connections);

    [[nodiscard]] bool is_opened() const;
    [[nodiscard]] std::string const &addr() const;
    [[nodiscard]] uint16_t port() const;
 private:
    fd::FileDescriptor _fd;
    std::string _addr;
    uint16_t _port = 0;
    bool _opened = false;
};
}  // namespace tcp

#endif //PROJECT_LIB_TCP_SERVER_H_
