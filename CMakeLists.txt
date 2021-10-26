cmake_minimum_required(VERSION 3.20)
project(math)

set(CMAKE_CXX_STANDARD 20)

add_library(math INTERFACE)
target_include_directories(math INTERFACE src)

add_executable(test_vector_operand test/vector_operand.cpp)
target_link_libraries(test_vector_operand PUBLIC math)

add_executable(vector_test test/vector_test.cpp)
target_link_libraries(vector_test PUBLIC math)

enable_testing()

add_test(NAME test_vector_operand COMMAND test_vector_operand)
