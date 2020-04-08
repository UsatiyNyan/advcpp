add_library(process STATIC
        src/process.cpp
        src/file_descriptor.cpp
        src/base_exception.cpp)
target_include_directories(process PUBLIC include)
