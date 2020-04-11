//
// Created by kira on 4/3/20.
//

#include "file_logger.h"

namespace log {

FileLogger::FileLogger(std::string const &filename) : _ofstream(filename, std::ios::out) {}

void FileLogger::flush() {
    _ofstream << std::flush;
}

void FileLogger::log(std::string const &s, Level level_) {
    if (level() <= level_) {
        _ofstream << log_array[static_cast<int>(level_)] << ' ' << s << std::endl;
    }
}


}  // namespace log
