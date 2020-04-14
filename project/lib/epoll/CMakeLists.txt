add_library(epoll STATIC
        src/epoll_server.cpp
        src/epoll_connection.cpp
        src/epoll_base.cpp
        )
target_link_libraries(epoll fd except)
target_include_directories(epoll PUBLIC include)
