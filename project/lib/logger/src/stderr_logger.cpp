//
// Created by kira on 4/3/20.
//

#include "stderr_logger.h"

#include <iostream>

namespace log {
void StderrLogger::flush() {
    std::cerr << std::flush;
}

void StderrLogger::log(std::string const &s, Level level_) {
    if (level() <= level_) {
        std::cerr << log_array[static_cast<int>(level_)] << ' ' << s << std::endl;
    }
}
}  // namespace log
