add_subdirectory(lib)
set(LIBS ${LIBS} PARENT_SCOPE)
add_executable(echo src/echo.cpp)
add_executable(advcpp src/main.cpp)

target_link_libraries(advcpp ${LIBS})
