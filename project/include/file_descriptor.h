//
// Created by kira on 3/23/20.
//

#ifndef PROJECT_INCLUDE_FILE_DESCRIPTOR_H_
#define PROJECT_INCLUDE_FILE_DESCRIPTOR_H_

#include "base_exception.h"


namespace process {
enum IO : int {
    READ, WRITE
};

class FileDescriptor {
 public:
    FileDescriptor();
    explicit FileDescriptor(int fd);
    FileDescriptor(FileDescriptor &&other) noexcept;
    ~FileDescriptor();

    int fd() const;
    void close();
    void link(FileDescriptor const &other);

    FileDescriptor &operator=(FileDescriptor &&other) noexcept;

 private:
    int _fd;
};

class Pipe {
 public:
    Pipe();
    const FileDescriptor &read() const;
    FileDescriptor &&read();
    const FileDescriptor &write() const;
    FileDescriptor &&write();
 private:
    FileDescriptor _read;
    FileDescriptor _write;
};
}  // namespace process


#endif  // PROJECT_INCLUDE_FILE_DESCRIPTOR_H_
