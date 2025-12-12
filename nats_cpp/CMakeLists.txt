cmake_minimum_required(VERSION 3.18)
project(cpp_nats_produser)

set(NATS_BUILD_EXAMPLES OFF CACHE BOOL "")
set(NATS_BUILD_TESTING  OFF CACHE BOOL "")
set(NATS_BUILD_STREAMING OFF CACHE BOOL "")

add_subdirectory(lib/nats.c)

add_executable(producer src/producer.cpp)

target_include_directories(producer PRIVATE
                            CMAKE_CURRENT_SOURCE_DIR/src/nats.c/src)
target_link_libraries(producer PRIVATE nats_static)


