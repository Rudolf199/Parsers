cmake_minimum_required(VERSION 3.10)
project(prac2_addition)

enable_testing()
set(CMAKE_CXX_STANDARD 14)


add_executable(
    prac2_addition
    addition_main.cpp
    src/addition.cpp
)
target_include_directories(prac2_addition PUBLIC ${CMAKE_SOURCE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(prac2_addition addition_lib)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
add_executable(
    Testing
    ${CMAKE_SOURCE_DIR}/test/test_main.cpp
    ${CMAKE_SOURCE_DIR}/test/test_addition.cpp
)
target_include_directories(Testing PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(Testing PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(Testing addition_lib)
target_link_libraries(Testing ${GTEST_LIBRARIES} Threads::Threads)
add_test(Testing ${CMAKE_SOURCE_DIR}/Test)
