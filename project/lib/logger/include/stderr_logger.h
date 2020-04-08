//
// Created by kira on 4/3/20.
//

#ifndef PROJECT_INCLUDE_STDERR_LOGGER_H_
#define PROJECT_INCLUDE_STDERR_LOGGER_H_

#include "base_logger.h"

namespace log {
class StderrLogger : public BaseLogger {
    void flush() override;
 private:
    void log(std::string s, Level level) override;
};
}  // namespace log


#endif //PROJECT_INCLUDE_STDERR_LOGGER_H_
