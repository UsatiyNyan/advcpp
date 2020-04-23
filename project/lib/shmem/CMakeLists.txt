add_library(shmem INTERFACE)
target_sources(shmem INTERFACE
        )
target_include_directories(shmem INTERFACE include src)

find_package(Threads)
target_link_libraries(shmem INTERFACE except ${CMAKE_THREAD_LIBS_INIT})
