cmake_minimum_required(VERSION 3.10)
project(earley_parser)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
add_executable(
    earley_parser
    src/Earley.cpp
)
target_include_directories(earley_parser PUBLIC ${CMAKE_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(earley_parser parser_lib)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
add_executable(
    Testing
    ${CMAKE_SOURCE_DIR}/tests/tests.cpp
)
target_include_directories(Testing PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(Testing PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(Testing parser_lib)
target_link_libraries(Testing ${GTEST_LIBRARIES} Threads::Threads)
add_test(Testing ${CMAKE_SOURCE_DIR}/Test)
set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "--coverage")