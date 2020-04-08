//
// Created by kira on 4/3/20.
//

#ifndef PROJECT_INCLUDE_FILE_LOGGER_H_
#define PROJECT_INCLUDE_FILE_LOGGER_H_

#include "base_logger.h"
#include <fstream>

namespace log {
class FileLogger : public BaseLogger {
 public:
    explicit FileLogger(std::string filename);
    ~FileLogger();
    void flush() override;
 private:
    void log(std::string s, Level level) override;
    std::ofstream _ofstream;
};
}  // namespace log

#endif //PROJECT_INCLUDE_FILE_LOGGER_H_
