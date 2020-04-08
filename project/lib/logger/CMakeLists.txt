add_library(log STATIC
        src/global_logger.cpp
        src/base_logger.cpp
        src/stdout_logger.cpp
        src/stderr_logger.cpp
        src/file_logger.cpp)

target_include_directories(log PUBLIC include)