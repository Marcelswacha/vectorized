cmake_minimum_required(VERSION 4.2)
project(vectorized)

set(CMAKE_CXX_STANDARD 20)

include_directories(collections utils)
add_executable(vectorized main.cpp)

# Enable AVX-512
target_compile_options(vectorized PRIVATE -mavx512f)
