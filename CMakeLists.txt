cmake_minimum_required(VERSION 3.16)
project(bucket)

set(CMAKE_CXX_STANDARD 17)

add_library(bucket INTERFACE)

target_include_directories(bucket INTERFACE inc/)

add_subdirectory(common)

target_link_libraries(bucket INTERFACE common)

add_subdirectory(tests)