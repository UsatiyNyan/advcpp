//
// Created by kira on 4/3/20.
//

#ifndef PROJECT_INCLUDE_BASE_LOGGER_H_
#define PROJECT_INCLUDE_BASE_LOGGER_H_

#include <string>

namespace log {
enum class Level {
    Debug, Info, Warn, Error
};

class BaseLogger {
 public:
    void debug(std::string s);
    void info(std::string s);
    void warn(std::string s);
    void error(std::string s);

    void set_level(Level level);
    Level level() const;

    virtual void flush() = 0;
 private:
    virtual void log(std::string s, Level level) = 0;
    Level _level = Level::Info;
};
}  // namespace log


#endif //PROJECT_INCLUDE_BASE_LOGGER_H_
