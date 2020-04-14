//
// Created by kira on 4/14/20.
//

#include "epoll_server.h"
#include "base_exception.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <utility>

using exception::Exception;

namespace epoll {
Server::Server(std::string ip, uint16_t port, ClientCallback on_read, ClientCallback on_write)
: _epoll(std::move(on_read), std::move(on_write)) {
    open(std::move(ip), port);
    _epoll.add_server(_server_fd);
}

void Server::open(std::string ip, uint16_t port) {
    auto listener = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listener == -1) {
        throw Exception("socket creation failed");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (bind(listener,
             reinterpret_cast<sockaddr *>(&addr),
             sizeof(addr)) == -1) {
        throw Exception("bind failed");
    }

    _addr = std::move(ip);
    _port = port;

    _opened = true;
    _server_fd = fd::FileDescriptor(listener);

    set_max_connections(Epoll::QUEUE_SIZE);
}

void Server::close() {
    _opened = false;
    _server_fd.close();
}

void Server::set_max_connections(uint32_t max_connections) {
    if (listen(_server_fd.fd(), max_connections) == -1) {
        _opened = false;
        throw Exception("listen error");
    }
}

void Server::set_read_cb(ClientCallback on_read) {
    _epoll.set_read_cb(std::move(on_read));
}

void Server::set_write_cb(ClientCallback on_write) {
    _epoll.set_write_cb(std::move(on_write));
}

void Server::spin_once() {
    _epoll.spin_once();
}

void Server::spin() {
    _epoll.spin();
}

bool Server::is_opened() const {
    return _opened;
}

std::string const &Server::addr() const {
    return _addr;
}

uint16_t Server::port() const {
    return _port;
}
}  // namespace epoll
