cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++1z")
set(CMAKE_CXX_COMPILER /usr/bin/g++-6)


MESSAGE("Compiler is ${CMAKE_CXX_COMPILER}")
#if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0) message(FATAL_ERROR "Host Clang version must be at least 5!")
#endif()


project(lzw CXX)
add_executable(lzw435 lzw_main.cpp lzw_original.cpp)
