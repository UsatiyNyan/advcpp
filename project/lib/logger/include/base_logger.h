//
// Created by kira on 4/3/20.
//

#ifndef PROJECT_INCLUDE_BASE_LOGGER_H_
#define PROJECT_INCLUDE_BASE_LOGGER_H_

#include <string>
#include <array>

namespace log {
enum class Level {
    Debug, Info, Warn, Error
};

static const std::array<std::string, 4> log_array = {"[DEBUG]", "[INFO]", "[WARN]", "[ERROR]"};

class BaseLogger {
 public:
    void debug(std::string const &s);
    void info(std::string const &s);
    void warn(std::string const &s);
    void error(std::string const &s);

    void set_level(Level level);
    Level level() const;

    virtual void flush() = 0;
 private:
    virtual void log(std::string const &s, Level level) = 0;
    Level _level = Level::Info;
};
}  // namespace log


#endif //PROJECT_INCLUDE_BASE_LOGGER_H_
