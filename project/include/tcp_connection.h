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
    Connection();
    Connection(process::FileDescriptor fd,
            std::string src_address, uint16_t src_port,
            std::string dst_address, uint16_t dst_port);
 public:
    Connection(std::string ip, short port);
    void connect(std::string ip, short port);
    void close();

    size_t write(const void *data, size_t size);
    void write_exact(const void *data, size_t size);
    size_t read(void *data, size_t size);
    void read_exact(void *data, size_t size);

    void set_timeout(int timeout = 0);

    bool is_opened() const;
    std::string const &dst_addr() const;
    std::string const &src_addr() const;
    uint16_t dst_port() const;
    uint16_t src_port() const;

    Connection(Connection &other) = delete;
 private:
    process::FileDescriptor _fd;
    std::string _dst_addr;
    std::string _src_addr;
    uint16_t _dst_port;
    uint16_t _src_port;
    bool _opened;
};
}  // namespace tcp

#endif //PROJECT_LIB_TCP_CONNECTION_H_
