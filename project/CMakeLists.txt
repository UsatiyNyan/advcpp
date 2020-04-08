add_subdirectory(lib)

add_executable(echo src/echo.cpp)

add_executable(advcpp src/main.cpp)

target_link_libraries(advcpp ${LIBS})
