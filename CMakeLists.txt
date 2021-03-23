cmake_minimum_required(VERSION 3.16)
project(TokenBucket)

set(CMAKE_CXX_STANDARD 17)

add_library(TokenBucket INTERFACE)

target_include_directories(TokenBucket INTERFACE inc/)

add_subdirectory(common)

target_link_libraries(TokenBucket INTERFACE common)

add_subdirectory(tests)