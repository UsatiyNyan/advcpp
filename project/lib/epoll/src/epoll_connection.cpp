//
// Created by kira on 4/14/20.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "base_exception.h"
#include "epoll_connection.h"


using exception::Exception;

namespace epoll {
Connection::Connection(fd::FileDescriptor fd) {
    sockaddr_in src_address{};
    sockaddr_in dst_address{};
    socklen_t address_size = sizeof(src_address);
    if (getsockname(fd.fd(),
                    reinterpret_cast<sockaddr *>(&src_address),
                    &address_size) == -1) {
        throw Exception("getsockname error");
    }
    if (getpeername(fd.fd(),
                reinterpret_cast<sockaddr *>(&dst_address),
                &address_size) == -1) {
        throw Exception("getpeername error");
    }

    _fd = std::move(fd);
    _src_addr = inet_ntoa(src_address.sin_addr);
    _src_port = ntohs(src_address.sin_port);
    _dst_addr = inet_ntoa(dst_address.sin_addr);
    _dst_port = ntohs(dst_address.sin_port);
    _opened = true;
}

Connection::Connection(Connection &&other) noexcept {
    _fd = std::move(other._fd);
    _dst_addr = std::move(other._dst_addr);
    _dst_port = other._dst_port;
    _src_addr = std::move(other._src_addr);
    _src_port = other._src_port;
    _opened = other._opened;
}

Connection &Connection::operator=(Connection &&other) noexcept {
    _fd = std::move(other._fd);
    _dst_addr = std::move(other._dst_addr);
    _dst_port = other._dst_port;
    _src_addr = std::move(other._src_addr);
    _src_port = other._src_port;
    _opened = other._opened;
    return *this;
}

Connection::Connection(std::string ip, uint16_t port) {
    connect(std::move(ip), port);
}

void Connection::connect(std::string ip, uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw Exception("socket creation failed");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (::connect(sock,
                  reinterpret_cast<sockaddr *>(&addr),
                  sizeof(addr)) == -1) {
        throw Exception("connection failed");
    }

    sockaddr_in client_address{};
    int client_address_size = sizeof(client_address);
    if (getsockname(sock,
                    reinterpret_cast<sockaddr *>(&client_address),
                    reinterpret_cast<socklen_t *>(&client_address_size)) == -1) {
        throw Exception("client info error");
    }


    _dst_addr = std::move(ip);
    _dst_port = port;
    _src_addr = inet_ntoa(client_address.sin_addr);
    _src_port = ntohs(client_address.sin_port);

    _opened = true;
    _fd = std::move(fd::FileDescriptor(sock));
}

void Connection::close() {
    _opened = false;
    _fd.close();
}

void Connection::read(size_t size) {
    _to_read += size;
}

void Connection::write(const void *data, size_t size) {
    _write_cache += std::string(static_cast<const char *>(data), size);
}

void Connection::try_read() {
    char buf[_to_read];
    ssize_t read_n = recv(_fd.fd(), buf, _to_read, 0);
    if (read_n == -1) {
        _opened = false;
        throw Exception("read error");
    }
    _to_read -= read_n;
    if (read_n == 0) {
        _opened = false;
        _to_read = 0;
        return;
    }
    _read_cache += buf;
}

void Connection::try_write() {
    ssize_t written = send(_fd.fd(), _write_cache.data(), _write_cache.size(), 0);
    if (written == -1) {
        _opened = false;
        throw Exception("write error");
    }
    if (written == 0) {
        _opened = false;
    }
    _write_cache.substr(written).swap(_write_cache);
}

size_t Connection::await_read(void *data) {
    while (is_readable()) {
        try_read();
    }
    size_t size = _read_cache.size();
    std::copy(_read_cache.begin(), _read_cache.end(), static_cast<char *>(data));
    std::string().swap(_read_cache);
    return size;
}

size_t Connection::await_write() {
    while (is_writable()) {
        try_write();
    }
    size_t size = _write_cache.size();
    std::string().swap(_write_cache);
    return size;
}

bool Connection::is_opened() const {
    return _opened;
}

bool Connection::is_readable() const {
    return _to_read > 0;
}

bool Connection::is_writable() const {
    return !_write_cache.empty();
}

std::string const &Connection::dst_addr() const {
    return _dst_addr;
}

std::string const &Connection::src_addr() const {
    return _src_addr;
}

uint16_t Connection::dst_port() const {
    return _dst_port;
}

uint16_t Connection::src_port() const {
    return _src_port;
}
}  // namespace epoll
