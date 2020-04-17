add_library(tcp STATIC
        src/tcp_server.cpp
        src/tcp_connection.cpp)
target_link_libraries(tcp fd except)
target_include_directories(tcp PUBLIC include)
