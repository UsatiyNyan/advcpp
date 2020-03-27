add_library(process STATIC
        lib/process.cpp
        lib/file_descriptor.cpp
        lib/base_exception.cpp)
target_include_directories(process PUBLIC include)
add_executable(advcpp src/main.cpp)
target_link_libraries(advcpp process)
