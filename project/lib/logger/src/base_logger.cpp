//
// Created by kira on 4/3/20.
//

#include "base_logger.h"

namespace log {
void BaseLogger::debug(std::string const & s) {
    log(s, Level::Debug);
}

void BaseLogger::info(std::string const & s) {
    log(s, Level::Info);
}

void BaseLogger::warn(std::string const & s) {
    log(s, Level::Warn);
}

void BaseLogger::error(std::string const & s) {
    log(s, Level::Error);
}

void BaseLogger::set_level(Level level) {
    _level = level;
}

Level BaseLogger::level() const {
    return _level;
}
}  // namespace log
