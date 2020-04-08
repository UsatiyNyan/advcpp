//
// Created by kira on 4/3/20.
//

#include "stderr_logger.h"

#include <iostream>

namespace log {
void StderrLogger::flush() {
    std::cerr << std::endl;
}

void StderrLogger::log(std::string s, Level level) {
    switch (level) {
        case Level::Debug:
            std::cerr << "[DEBUG] ";
            break;
        case Level::Info:
            std::cerr << "[INFO] ";
            break;
        case Level::Warn:
            std::cerr << "[WARN] ";
            break;
        case Level::Error:
            std::cerr << "[ERROR] ";
            break;
        default:
            break;
    }
    std::cerr << s << '\n';
}
}  // namespace log
