//
// Created by kira on 3/27/20.
//

#include "global_logger.h"

namespace log {
void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) {
    _global_logger = std::move(logger);
}

Logger &Logger::get_instance() {
    static std::unique_ptr<Logger> instance(new Logger);
    if (!instance->_global_logger) {
        instance->set_global_logger(create_stdout_logger());
    }
    return *instance;
}

BaseLogger &Logger::get_global_logger() {
    if (!_global_logger) {
        set_global_logger(create_stdout_logger());
    }
    return *_global_logger;
}

std::unique_ptr<FileLogger> create_file_logger(std::string const &filename) {
    auto file_logger = std::make_unique<FileLogger>(filename);
    file_logger->set_level(log::Level::Debug);
    return file_logger;
}

std::unique_ptr<StdoutLogger> create_stdout_logger() {
    return std::make_unique<StdoutLogger>();
}

std::unique_ptr<StderrLogger> create_stderr_logger() {
    auto stderr_logger = std::make_unique<StderrLogger>();
    stderr_logger->set_level(log::Level::Error);
    return stderr_logger;
}

void debug(std::string const &s) {
    Logger::get_instance().get_global_logger().debug(s);
}

void info(std::string const &s) {
    Logger::get_instance().get_global_logger().info(s);
}

void warn(std::string const &s) {
    Logger::get_instance().get_global_logger().warn(s);
}

void error(std::string const &s) {
    Logger::get_instance().get_global_logger().error(s);
}

void set_logger_level(Level level) {
    log::Logger::get_instance().get_global_logger().set_level(level);
}
}  // namespace log