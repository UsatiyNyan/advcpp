//
// Created by kira on 3/27/20.
//

#include <iostream>
#include <utility>

#include "global_logger.h"

namespace log {
std::unique_ptr<Logger> Logger::_instance = std::unique_ptr<Logger>(nullptr);

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

FileLogger::FileLogger(std::string filename) : _ofstream(std::move(filename), std::ios::out) {}

void FileLogger::flush() {
    _ofstream << std::endl;
}

FileLogger::~FileLogger() {
}

void FileLogger::log(std::string s, Level level) {
    switch (level) {
        case Level::Debug:
            _ofstream << "[DEBUG] ";
            break;
        case Level::Info:
            _ofstream << "[INFO] ";
            break;
        case Level::Warn:
            _ofstream << "[WARN] ";
            break;
        case Level::Error:
            _ofstream << "[ERROR] ";
            break;
        default:
            break;
    }
    _ofstream << s << std::endl;
}

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