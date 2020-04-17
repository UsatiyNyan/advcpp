add_library(fd STATIC
        src/file_descriptor.cpp
        )
target_include_directories(fd PUBLIC include)
add_library(except STATIC
        src/base_exception.cpp
        )
target_include_directories(except PUBLIC include)
add_library(process STATIC
        src/process.cpp
        )
target_link_libraries(process fd except)
target_include_directories(process PUBLIC include)
