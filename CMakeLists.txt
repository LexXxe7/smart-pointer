cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++14 -stdlib=libc++)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SmartPointer)

add_executable(SmartPointer src/main.cpp)