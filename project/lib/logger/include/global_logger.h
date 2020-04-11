//
// Created by kira on 3/27/20.
//

#ifndef PROJECT_INCLUDE_GLOBAL_LOGGER_H_
#define PROJECT_INCLUDE_GLOBAL_LOGGER_H_

#include "base_logger.h"
#include "stdout_logger.h"
#include "stderr_logger.h"
#include "file_logger.h"

#include <memory>

namespace log {
class Logger {
 public:
    void set_global_logger(std::unique_ptr<BaseLogger> logger);
    static Logger &get_instance();
    BaseLogger &get_global_logger();

 private:
    Logger() = default;
    std::unique_ptr<BaseLogger> _global_logger;
};

std::unique_ptr<FileLogger> create_file_logger(std::string const &filename);
std::unique_ptr<StdoutLogger> create_stdout_logger();
std::unique_ptr<StderrLogger> create_stderr_logger();
void set_logger_level(Level level);

void debug(std::string const &s);
void info(std::string const &s);
void warn(std::string const &s);
void error(std::string const &s);
}  // namespace log

#endif  //PROJECT_INCLUDE_GLOBAL_LOGGER_H_
