add_library(process STATIC
        lib/process.cpp
        lib/file_descriptor.cpp
        lib/base_exception.cpp)

target_include_directories(process PUBLIC include)
add_executable(echo src/echo.cpp)
add_library(log STATIC
        lib/global_logger.cpp
        lib/base_logger.cpp
        lib/stdout_logger.cpp
        lib/stderr_logger.cpp
        lib/file_logger.cpp
        )

target_include_directories(log PUBLIC include)
add_executable(advcpp src/main.cpp)
target_link_libraries(advcpp process log)
