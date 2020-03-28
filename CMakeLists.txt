cmake_minimum_required(VERSION 3.13)
project(advcpp)
include(gtest.cmake)

set(CMAKE_CXX_STANDARD 17)

#   examples
add_subdirectory(examples)

#   libs &
#   main program
add_subdirectory(project)

#   tests
#   GTest needs threading support
add_subdirectory(tests)
