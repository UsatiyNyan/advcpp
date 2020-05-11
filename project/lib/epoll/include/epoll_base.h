//
// Created by kira on 4/14/20.
//

#ifndef PROJECT_LIB_EPOLL_INCLUDE_EPOLL_BASE_H_
#define PROJECT_LIB_EPOLL_INCLUDE_EPOLL_BASE_H_


#include "file_descriptor.h"
#include "epoll_connection.h"

#include <functional>
#include <map>

namespace epoll {
using AcceptCallback = std::function<void()>;
using ClientCallback = std::function<void(Connection &)>;

class Epoll {
 public:
    Epoll(ClientCallback on_read, ClientCallback on_write);

    void add_server(const fd::FileDescriptor &fd, AcceptCallback accept_callback = {});
    void add(const Connection &connection, uint32_t events);
    void mod(const Connection &connection, uint32_t events);
    void del(const Connection &connection);

    void spin_once();
    void spin();

    void set_read_cb(ClientCallback on_read);
    void set_write_cb(ClientCallback on_write);

 private:
    void ctl(int fd, uint32_t events, int operation);
    void default_accept();
    void handle_client(int clinet_fd, unsigned event);

    fd::FileDescriptor _epoll_fd;
    int _server_fd = -1;

    AcceptCallback _accept;
    ClientCallback _on_read;
    ClientCallback _on_write;

    std::map<int, Connection> _connections;

};
}  // namespace epoll


#endif //PROJECT_LIB_EPOLL_INCLUDE_EPOLL_BASE_H_
