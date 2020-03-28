//
// Created by kira on 3/27/20.
//

#include "global_logger.h"

namespace log {
std::unique_ptr<Logger> Logger::_instance = std::unique_ptr<Logger>(nullptr);
BaseLogger &Logger::get_global_logger() {
    return *_global_logger;
}

void Logger::set_global_logger(std::unique_ptr<BaseLogger> logger) {
    _global_logger = std::move(logger);
}

Logger &Logger::get_instance() {
    if (!_instance) {
        _instance = std::unique_ptr<Logger>(new Logger(
                std::make_unique<StdoutLogger>()));
    }
    return *_instance;
}

Logger::Logger(std::unique_ptr<BaseLogger> logger) : _global_logger(std::move(logger)) {}


std::unique_ptr<FileLogger> create_file_logger(std::string filename) {
    auto file_logger = std::make_unique<FileLogger>(std::move(filename));
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

void debug(std::string s) {
    Logger::get_instance().get_global_logger().debug(std::move(s));
}

void info(std::string s) {
    Logger::get_instance().get_global_logger().info(std::move(s));
}

void warn(std::string s) {
    Logger::get_instance().get_global_logger().warn(std::move(s));
}

void error(std::string s) {
    Logger::get_instance().get_global_logger().error(std::move(s));
}

void set_logger_level(Level level) {
    log::Logger::get_instance().get_global_logger().set_level(level);
}
}  // namespace log