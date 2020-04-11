add_library(logger STATIC
        src/global_logger.cpp
        src/base_logger.cpp
        src/stdout_logger.cpp
        src/stderr_logger.cpp
        src/file_logger.cpp)

target_include_directories(logger PUBLIC include)
