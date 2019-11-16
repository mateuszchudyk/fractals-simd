cmake_minimum_required(VERSION 3.10)

project(fractals-simd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -msse3 -mavx2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -msse3 -mavx2")

add_library(fractals-sse3 STATIC sse3.cpp)
add_library(fractals-avx2 STATIC avx2.cpp)

add_executable(demo demo.cpp)

# Use AVX2 by defualt
target_link_libraries(demo fractals-avx2)
