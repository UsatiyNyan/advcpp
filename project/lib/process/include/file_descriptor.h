//
// Created by kira on 3/23/20.
//

#ifndef PROJECT_INCLUDE_FILE_DESCRIPTOR_H_
#define PROJECT_INCLUDE_FILE_DESCRIPTOR_H_

#include "base_exception.h"


namespace fd {
enum IO : int {
    READ, WRITE
};

class FileDescriptor {
 public:
    FileDescriptor();
    explicit FileDescriptor(int fd);
    FileDescriptor(FileDescriptor &&other) noexcept;
    FileDescriptor(FileDescriptor const &other) = delete;
    ~FileDescriptor();

    [[nodiscard]] int fd() const;
    void close();
    void link(FileDescriptor const &other);

    FileDescriptor &operator=(FileDescriptor &&other) noexcept;
    FileDescriptor &operator=(FileDescriptor const &other) = delete;

 private:
    int _fd;
};

class Pipe {
 public:
    Pipe();
    [[nodiscard]] const FileDescriptor &read() const;
    FileDescriptor &&read();
    [[nodiscard]] const FileDescriptor &write() const;
    FileDescriptor &&write();
 private:
    FileDescriptor _read;
    FileDescriptor _write;
};
}  // namespace process


#endif  // PROJECT_INCLUDE_FILE_DESCRIPTOR_H_
