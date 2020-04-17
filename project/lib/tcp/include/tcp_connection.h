//
// Created by kira on 3/31/20.
//

#ifndef PROJECT_LIB_TCP_CONNECTION_H_
#define PROJECT_LIB_TCP_CONNECTION_H_

#include <string>

#include "file_descriptor.h"

namespace tcp {
class Connection {
    friend class Server;
    Connection(fd::FileDescriptor fd,
            std::string src_address, uint16_t src_port,
            std::string dst_address, uint16_t dst_port);
 public:
    Connection(Connection &&other) noexcept;
    Connection &operator=(Connection &&other) noexcept;
    Connection(std::string ip, uint16_t port);
    void connect(std::string ip, uint16_t port);
    void close();

    size_t write(const void *data, size_t size);
    void write_exact(const void *data, size_t size);
    size_t read(void *data, size_t size);
    void read_exact(void *data, size_t size);

    void set_timeout(int timeout = 0);

    [[nodiscard]] bool is_opened() const;
    [[nodiscard]] std::string const &dst_addr() const;
    [[nodiscard]] std::string const &src_addr() const;
    [[nodiscard]] uint16_t dst_port() const;
    [[nodiscard]] uint16_t src_port() const;
 private:
    fd::FileDescriptor _fd;
    std::string _dst_addr;
    std::string _src_addr;
    uint16_t _dst_port = 0;
    uint16_t _src_port = 0;
    bool _opened = false;
};
}  // namespace tcp

#endif //PROJECT_LIB_TCP_CONNECTION_H_
