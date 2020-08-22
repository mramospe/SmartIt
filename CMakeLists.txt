cmake_minimum_required(VERSION 2.8.9)
project(SmartIt)

option(INSTALL_TESTS "Whether to install the test scripts or not" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(smartit INTERFACE)

target_include_directories(smartit INTERFACE include)

if(INSTALL_TESTS)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test)
    include_directories(include)
    add_executable(smartit_test test/test.cpp)
    set_target_properties(smartit_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
endif(INSTALL_TESTS)
