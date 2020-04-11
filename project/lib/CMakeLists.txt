add_subdirectory(process)
add_subdirectory(tcp)
add_subdirectory(logger)
set(LIBS process fd except tcp logger PARENT_SCOPE)

