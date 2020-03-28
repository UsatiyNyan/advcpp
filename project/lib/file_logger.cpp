//
// Created by kira on 4/3/20.
//

#include "file_logger.h"

namespace log {

FileLogger::FileLogger(std::string filename) : _ofstream(std::move(filename), std::ios::out) {}

void FileLogger::flush() {
    _ofstream << std::endl;
}

FileLogger::~FileLogger() {
}

void FileLogger::log(std::string s, Level level) {
    switch (level) {
        case Level::Debug:
            _ofstream << "[DEBUG] ";
            break;
        case Level::Info:
            _ofstream << "[INFO] ";
            break;
        case Level::Warn:
            _ofstream << "[WARN] ";
            break;
        case Level::Error:
            _ofstream << "[ERROR] ";
            break;
        default:
            break;
    }
    _ofstream << s << std::endl;
}


}  // namespace log
