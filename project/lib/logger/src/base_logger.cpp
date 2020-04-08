//
// Created by kira on 4/3/20.
//

#include "base_logger.h"

namespace log {
void BaseLogger::debug(std::string s) {
    if (_level <= Level::Debug) {
        log(std::move(s), Level::Debug);
    }
}

void BaseLogger::info(std::string s) {
    if (_level <= Level::Info) {
        log(std::move(s), Level::Info);
    }
}

void BaseLogger::warn(std::string s) {
    if (_level <= Level::Warn) {
        log(std::move(s), Level::Warn);
    }
}

void BaseLogger::error(std::string s) {
    if (_level <= Level::Error) {
        log(std::move(s), Level::Error);
    }
}

void BaseLogger::set_level(Level level) {
    _level = level;
}

Level BaseLogger::level() const {
    return _level;
}
}  // namespace log
