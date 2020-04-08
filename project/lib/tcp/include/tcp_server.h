//
// Created by kira on 3/31/20.
//

#ifndef PROJECT_LIB_TCP_SERVER_H_
#define PROJECT_LIB_TCP_SERVER_H_

#include "file_descriptor.h"
#include "tcp_connection.h"
#include <memory>

namespace tcp {
using ConnectionPtr = std::unique_ptr<Connection>;

class Server {
 public:
    Server(std::string ip, short port);
    ConnectionPtr accept();

    void open(std::string ip, short port);
    void close();

    void set_max_connections(int max_connections);

    bool is_opened() const;
    std::string const &addr() const;
    uint16_t port() const;
 private:
    process::FileDescriptor _fd;
    std::string _addr;
    uint16_t _port;
    bool _opened;
};
}  // namespace tcp

#endif //PROJECT_LIB_TCP_SERVER_H_
