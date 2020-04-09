add_subdirectory(process)
add_subdirectory(tcp)

set(LIBS process fd except tcp PARENT_SCOPE)