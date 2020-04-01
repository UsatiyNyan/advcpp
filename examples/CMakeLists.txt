add_executable(client example_client.cpp)
target_link_libraries(client tcp log)
add_executable(server example_server.cpp)
target_link_libraries(server tcp log)
