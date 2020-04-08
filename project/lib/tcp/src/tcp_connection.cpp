//
// Created by kira on 3/31/20.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp_connection.h"
#include "base_exception.h"

namespace tcp {
Connection::Connection()
: _fd(),
  _src_addr(), _src_port(),
  _dst_addr(), _dst_port(),
  _opened(true) {}

Connection::Connection(process::FileDescriptor fd,
                       std::string src_address, uint16_t src_port,
                       std::string dst_address, uint16_t dst_port)
        : _fd(std::move(fd)),
          _src_addr(std::move(src_address)), _src_port(src_port),
          _dst_addr(std::move(dst_address)), _dst_port(dst_port),
          _opened(true) {}

Connection::Connection(std::string ip, short port) : Connection() {
    connect(std::move(ip), port);
}

void Connection::connect(std::string ip, short port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw process::Exception("socket creation failed");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (::connect(sock,
            reinterpret_cast<sockaddr *>(&addr),
            sizeof(addr)) == -1) {
        throw process::Exception("connection failed");
    }

    sockaddr_in client_address{};
    int client_address_size = sizeof(client_address);
    if (getsockname(sock,
            reinterpret_cast<sockaddr *>(&client_address),
            reinterpret_cast<socklen_t *>(&client_address_size)) == -1) {
        throw process::Exception("client info error");
    }


    _dst_addr = std::move(ip);
    _dst_port = port;
    _src_addr = inet_ntoa(client_address.sin_addr);
    _src_port = ntohs(client_address.sin_port);

    _opened = true;
    _fd = std::move(process::FileDescriptor(sock));
}

void Connection::close() {
    _opened = false;
    _fd.close();
}

size_t Connection::write(const void *data, size_t size) {
    ssize_t written = send(_fd.fd(), data, size, 0);
    if (written == -1) {
        _opened = false;
        throw process::Exception("write error");
    }
    return written;
}

void Connection::write_exact(const void *data, size_t size) {
    size_t written = 0;
    while (size != written) {
        written += write(static_cast<const char *>(data) + written, size - written);
    }
}

size_t Connection::read(void *data, size_t size) {
    ssize_t read_n = ::recv(_fd.fd(), data, size, 0);
    if (read_n == -1) {
        _opened = false;
        throw process::Exception("read error");
    }
    return read_n;
}

void Connection::read_exact(void *data, size_t size) {
    size_t read_n = 0;
    while (size != read_n) {
        read_n += read(static_cast<char *>(data) + read_n, size - read_n);
    }
}

void Connection::set_timeout(int timeout) {
    timeval _timeout{.tv_sec = timeout, .tv_usec = 0};
    if (setsockopt(_fd.fd(),
                   SOL_SOCKET,
                   SO_SNDTIMEO,
                   &_timeout,
                   sizeof(_timeout)) == -1) {
        throw process::Exception("timeout set failed");
    }
    if (setsockopt(_fd.fd(),
                   SOL_SOCKET,
                   SO_RCVTIMEO,
                   &_timeout,
                   sizeof(_timeout)) == -1) {
        throw process::Exception("timeout set failed");
    }
}

bool Connection::is_opened() const {
    return _opened;
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
}  // namespace tcp
