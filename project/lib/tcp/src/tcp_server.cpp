//
// Created by kira on 3/31/20.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp_server.h"


using exception::Exception;

namespace tcp {
Server::Server(std::string ip, uint16_t port) {
    open(std::move(ip), port);
}

Server::~Server() {
    close();
}

void Server::open(std::string ip, uint16_t port) {
    auto listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    _fd = fd::FileDescriptor(listener);

    set_max_connections(1);
}

void Server::close() {
    _opened = false;
    _fd.close();
}

Connection Server::accept() {
    sockaddr_in client_address{};
    int client_address_size = sizeof(client_address);
    auto sock = ::accept(_fd.fd(),
            reinterpret_cast<sockaddr *>(&client_address),
            reinterpret_cast<socklen_t *>(&client_address_size));
    if (sock == -1) {
        throw Exception("accept error");
    }
    return Connection(std::move(fd::FileDescriptor(sock)),_addr, _port,
            inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
}

void Server::set_max_connections(int max_connections) {
    if (listen(_fd.fd(), max_connections) == -1) {
        _opened = false;
        throw Exception("listen error");
    }
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
}  // namespace tcp
