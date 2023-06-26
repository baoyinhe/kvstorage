cmake_minimum_required(VERSION 3.12)
project(kvSDK_Example)

set(CMAKE_CXX_STANDARD 11)

add_executable(kvSDK_Example src/main.cpp)

target_link_libraries(kvSDK_Example kvSDK)