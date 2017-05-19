cmake_minimum_required(VERSION 3.5)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(MSSC_SOURCE_FILES
        MSSC.cpp
        MSSC.h)

add_library(MSSC ${MSSC_SOURCE_FILES})