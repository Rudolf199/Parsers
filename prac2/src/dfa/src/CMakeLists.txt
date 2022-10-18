cmake_minimum_required(VERSION 3.10)
project(dfa)
set(SOURCE_LIB DFA.cpp)
add_library(dfa_lib SHARED ${SOURCE_LIB})