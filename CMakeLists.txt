cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_FLAGS "-std=c++1z")
set(CMAKE_CXX_COMPILER_ID "clang-5.0")
set(CMAKE_CXX_COMPILER -DCMAKE_CXX_COMPILER=/usr/local/Cellar/llvm/5.0.0/bin/clang-5.0)

#MESSAGE("Compiler is ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
#if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0) message(FATAL_ERROR "Host Clang version must be at least 5!")
#endif()


project(lzw CXX)
add_executable(lzw435 lzw_main.cpp lzw_original.cpp)


