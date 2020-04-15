//
// Created by kira on 4/14/20.
//

#ifndef PROJECT_LIB_EPOLL_SRC_CONNECTION_H_
#define PROJECT_LIB_EPOLL_SRC_CONNECTION_H_


#include "file_descriptor.h"

#include <string>
#include <map>
#include <vector>

namespace epoll {
class Connection {
    friend class Epoll;
 public:
    Connection();
    explicit Connection(fd::FileDescriptor fd);
    Connection(Connection &&other) noexcept;
    Connection &operator=(Connection &&other) noexcept;
    Connection(std::string ip, uint16_t port);
    void connect(std::string ip, uint16_t port);
    void close();

    void read(size_t size);
    void write(const void *data, size_t size);
    size_t await_read(void *data);
    size_t await_write();

    [[nodiscard]] bool is_opened() const;
    [[nodiscard]] bool is_writable() const;
    [[nodiscard]] bool is_readable() const;
    [[nodiscard]] std::string const &dst_addr() const;
    [[nodiscard]] std::string const &src_addr() const;
    [[nodiscard]] uint16_t dst_port() const;
    [[nodiscard]] uint16_t src_port() const;
 private:
    void try_read();
    void try_write();

    fd::FileDescriptor _fd;
    std::string _dst_addr;
    std::string _src_addr;
    uint16_t _dst_port;
    uint16_t _src_port;
    bool _opened;

    std::string _write_cache;
    std::string _read_cache;
    size_t _to_write;
    size_t _to_read;
};
}  // namespace epoll

#endif //PROJECT_LIB_EPOLL_SRC_CONNECTION_H_