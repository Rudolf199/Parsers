cmake_minimum_required(VERSION 3.10)
project(fdfa)
set(SOURCE_LIB FDFA.cpp)
add_library(fdfa_lib SHARED ${SOURCE_LIB})