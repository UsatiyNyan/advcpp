//
// Created by kira on 4/14/20.
//

#include "base_exception.h"
#include "epoll_base.h"

#include <sys/epoll.h>
#include <netinet/in.h>

#include <utility>


using exception::Exception;

namespace epoll {
const size_t QUEUE_SIZE = 128;

Epoll::Epoll(ClientCallback on_read, ClientCallback on_write)
: _on_read(std::move(on_read)),
  _on_write(std::move(on_write)),
  _epoll_fd(epoll_create(0xD1C)) {
    if (_epoll_fd.fd() == -1) {
        throw Exception("epoll creation failed");
    }
}

void Epoll::add_server(fd::FileDescriptor const & server_fd, AcceptCallback accept_callback) {
    if (_server_fd != -1) {
        ctl(_server_fd, 0, EPOLL_CTL_DEL);
        std::map<int, Connection>().swap(_connections);
    }
    ctl(server_fd.fd(), EPOLLIN, EPOLL_CTL_ADD);
    _server_fd = server_fd.fd();
    if (!accept_callback) {
        accept_callback = std::bind(&Epoll::default_accept, this);
    }
    _accept = std::move(accept_callback);
}

void Epoll::add(Connection const &connection, uint32_t events) {
    ctl(connection._fd.fd(), events, EPOLL_CTL_ADD);
}

void Epoll::mod(Connection const &connection, uint32_t events) {
    ctl(connection._fd.fd(), events, EPOLL_CTL_MOD);
}

void Epoll::del(Connection const &connection) {
    ctl(connection._fd.fd(), 0, EPOLL_CTL_DEL);
}

void Epoll::ctl(int fd, uint32_t events, int operation) {
    epoll_event event{};
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(_epoll_fd.fd(), operation, fd, &event) == -1) {
        throw Exception("epoll_ctl error on operation: " + std::to_string(operation));
    }
}

void Epoll::spin_once() {
    if (_server_fd == -1) {
        throw Exception("polling events w/o server");
    }
    std::vector<epoll_event> epoll_events(QUEUE_SIZE);
    int nfds = epoll_wait(_epoll_fd.fd(), epoll_events.data(), QUEUE_SIZE, -1);
    if (nfds == -1) {
        if (errno == EINTR) {
            return spin_once();
        }
        throw Exception("epoll_wait failed");
    }

    for (int i = 0; i < nfds; ++i) {
        int fd = epoll_events[i].data.fd;
        uint32_t events = epoll_events[i].events;
        if (fd == _server_fd) {
            _accept();
        } else {
            handle_client(fd, events);
        }
    }
}

void Epoll::spin() {
    while (true) {
        spin_once();
    }
}

void Epoll::set_read_cb(ClientCallback on_read) {
    _on_read = std::move(on_read);
}

void Epoll::set_write_cb(ClientCallback on_write) {
    _on_write = std::move(on_write);
}

void Epoll::default_accept() {
    int client_fd = ::accept(_server_fd,nullptr,nullptr);
    if (client_fd == -1) {
        throw Exception("accept error");
    }
    _connections[client_fd] = Connection(fd::FileDescriptor(client_fd));
    add(_connections[client_fd], EPOLLIN);
}

void Epoll::handle_client(int client_fd, unsigned event) {
    Connection &connection = _connections[client_fd];
    if (event & EPOLLHUP || event & EPOLLERR || !connection.is_opened()) {
        del(connection);
        _connections.erase(client_fd);
        return;
    }
    if (event & EPOLLIN) {
        _on_read(connection);
        if (!connection.is_readable()) {
            mod(connection, EPOLLOUT);
            return;
        }
    } else if (event & EPOLLOUT) {
        _on_write(connection);
        if (!connection.is_writable()) {
            mod(connection, EPOLLIN);
            return;
        }
    }
}
}  // namespace epoll
