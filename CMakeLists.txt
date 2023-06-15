cmake_minimum_required(VERSION 3.12)
project(cpp-test VERSION 0.0.0)


set(source_dir ${CMAKE_SOURCE_DIR}/source)
set(include_dir ${CMAKE_SOURCE_DIR}/include)

include_directories(${include_dir})

add_executable(cpp_test ${source_dir}/main.cpp)
