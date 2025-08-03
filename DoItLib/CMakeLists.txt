cmake_minimum_required(VERSION 3.21)
project(DoItLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DoItLib SHARED
    src/doit.h src/doit.cpp
)

target_include_directories(DoItLib PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/lib
)

set_target_properties(DoItLib PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)
