//
// Created by kira on 4/14/20.
//

#ifndef PROJECT_LIB_EPOLL_SRC_SERVER_H_
#define PROJECT_LIB_EPOLL_SRC_SERVER_H_

#include "file_descriptor.h"
#include "epoll_connection.h"
#include "epoll_base.h"

#include <string>

namespace epoll {
class Server {
 public:
    Server(std::string ip, uint16_t port, ClientCallback on_read, ClientCallback on_write);

    void open(std::string ip, uint16_t port);
    void close();

    void set_max_connections(uint32_t max_connections);
    void set_read_cb(ClientCallback on_read);
    void set_write_cb(ClientCallback on_write);

    void spin_once();
    void spin();

    [[nodiscard]] bool is_opened() const;
    [[nodiscard]] std::string const &addr() const;
    [[nodiscard]] uint16_t port() const;
 private:
    fd::FileDescriptor _server_fd;
    Epoll _epoll;

    uint32_t _max_conns = 0;

    std::string _addr;
    uint16_t _port = 0;

    bool _opened = false;
};
}  // namespace epoll


#endif //PROJECT_LIB_EPOLL_SRC_SERVER_H_
