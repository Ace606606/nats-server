cmake_minimum_required(VERSION 3.10)
project(cpp_nats_produser CXX)

set(NATS_BUILD_EXAMPLES OFF CACHE BOOL "")
set(NATS_BUILD_TESTING  OFF CACHE BOOL "")
set(NATS_BUILD_STREAMING OFF CACHE BOOL "")

add_subdirectory(nats.c)

add_executable(producer src/producer.cpp)
target_include_directories(producer PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/nats.c/src)
target_link_libraries(producer PRIVATE nats_static)
