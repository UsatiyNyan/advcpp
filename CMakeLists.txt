cmake_minimum_required(VERSION 3.13)
project(advcpp)
include(gtest.cmake)

set(CMAKE_CXX_STANDARD 17)

#   libs &
#   main program
add_subdirectory(project)

#   examples
add_executable(server example/server.cpp)
target_link_libraries(server ${LIBS})
add_executable(client example/client.cpp)
target_link_libraries(client ${LIBS})
add_executable(epoll_server example/epoll_server.cpp)
target_link_libraries(epoll_server ${LIBS})
add_executable(epoll_client example/epoll_client.cpp)
target_link_libraries(epoll_client ${LIBS})

#   tests
#   GTest needs threading support
add_subdirectory(tests)
