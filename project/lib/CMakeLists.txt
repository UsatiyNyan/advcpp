add_subdirectory(process)
add_subdirectory(tcp)

set(LIBS process tcp PARENT_SCOPE)