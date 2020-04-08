//
// Created by kira on 4/3/20.
//

#include "stdout_logger.h"

#include <iostream>

namespace log {
void StdoutLogger::flush() {
    std::cout << std::endl;
}

void StdoutLogger::log(std::string s, Level level) {
    switch (level) {
        case Level::Debug:
            std::cout << "[DEBUG] ";
            break;
        case Level::Info:
            std::cout << "[INFO] ";
            break;
        case Level::Warn:
            std::cout << "[WARN] ";
            break;
        case Level::Error:
            std::cout << "[ERROR] ";
            break;
        default:
            break;
    }
    std::cout << s << '\n';
}
}  // namespace log
