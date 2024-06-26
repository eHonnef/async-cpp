cmake_minimum_required(VERSION 3.27)
project(async_cpp)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_compile_options(-fcoroutines)

add_executable(async_cpp
        include/async/Async.hpp
        examples/main.cpp)
