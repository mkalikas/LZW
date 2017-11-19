cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++1z")

#set(CMAKE_CXX_COMPILER /usr/bin/g++-6)
# location of compiler on local MacOS machine
set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc@6/6.4.0/bin/g++-6)
set(CMAKE_CXX_STANDARD_REQUIRED c++17)
MESSAGE("Compiler is ${CMAKE_CXX_COMPILER}")

project(lzw CXX)

add_executable(lzw435 lzw435_main.cpp lzw435.cpp)
