add_subdirectory(process)
add_subdirectory(tcp)
add_subdirectory(logger)
add_subdirectory(epoll)
set(LIBS process fd except tcp logger epoll PARENT_SCOPE)

