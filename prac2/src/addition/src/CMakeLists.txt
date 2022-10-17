cmake_minimum_required(VERSION 3.10)
project(addition)
set(SOURCE_LIB addition.cpp)
add_library(addition_lib SHARED ${SOURCE_LIB})