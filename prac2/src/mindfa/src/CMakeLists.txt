cmake_minimum_required(VERSION 3.10)
project(min)
set(SOURCE_LIB minDFA.cpp)
add_library(min_lib SHARED ${SOURCE_LIB})