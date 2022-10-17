cmake_minimum_required(VERSION 3.10)
project(epsilon)
set(SOURCE_LIB epsilon.cpp)
add_library(epsilon_lib SHARED ${SOURCE_LIB})