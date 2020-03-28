//
// Created by kira on 3/27/20.
//

#ifndef PROJECT_INCLUDE_GLOBAL_LOGGER_H_
#define PROJECT_INCLUDE_GLOBAL_LOGGER_H_

#include <fstream>
#include <iostream>
#include <memory>

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

class StdoutLogger : public BaseLogger {
    void flush() override;
 private:
    void log(std::string s, Level level) override;
};

class StderrLogger : public BaseLogger {
    void flush() override;
 private:
    void log(std::string s, Level level) override;
};

class FileLogger : public BaseLogger {
 public:
    explicit FileLogger(std::string filename);
    ~FileLogger();
    void flush() override;
 private:
    void log(std::string s, Level level) override;
    std::ofstream _ofstream;
};

class Logger {
 public:
    BaseLogger &get_global_logger();
    void set_global_logger(std::unique_ptr<BaseLogger> logger);
    static Logger &get_instance();

 private:
    explicit Logger(std::unique_ptr<BaseLogger> logger);

    std::unique_ptr<BaseLogger> _global_logger;
    static std::unique_ptr<Logger> _instance;
};

std::unique_ptr<FileLogger> create_file_logger(std::string filename);
std::unique_ptr<StdoutLogger> create_stdout_logger();
std::unique_ptr<StderrLogger> create_stderr_logger();
void set_logger_level(Level level);

void debug(std::string s);
void info(std::string s);
void warn(std::string s);
void error(std::string s);
}  // namespace log

#endif  //PROJECT_INCLUDE_GLOBAL_LOGGER_H_
