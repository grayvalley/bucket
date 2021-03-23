cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_token_bucket test_token_bucket.cc queue.hh)

target_link_libraries(test_token_bucket TokenBucket)
target_link_libraries(test_token_bucket gtest gtest_main)
target_link_libraries(test_token_bucket pthread)