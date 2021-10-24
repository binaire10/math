cmake_minimum_required(VERSION 3.20)
project(math)

set(CMAKE_CXX_STANDARD 20)

add_library(math INTERFACE)
target_include_directories(math INTERFACE src)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC math)
