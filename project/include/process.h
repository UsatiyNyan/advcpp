//
// Created by kira on 07.03.2020.
//

#ifndef PROJECT_INCLUDE_PROCESS_H_
#define PROJECT_INCLUDE_PROCESS_H_

#include <string>

#include "file_descriptor.h"

namespace process {
class Process {
 public:
    explicit Process(const std::string &path);

    ~Process();

    size_t write(const void *data, size_t len);

    void write_exact(const void *data, size_t len);

    size_t read(void *data, size_t len);

    void read_exact(void *data, size_t len);

    [[nodiscard]] bool is_readable() const;

    void close_stdin();

    void close();

 private:
    pid_t _pid;
    int _status;
    FileDescriptor _pipein;
    FileDescriptor _pipeout;
    bool _readable;
};
}  // namespace process

#endif  // PROJECT_INCLUDE_PROCESS_H_
