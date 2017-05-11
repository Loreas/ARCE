cmake_minimum_required(VERSION 3.7)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(MSSC_SOURCE_FILES
        mssc.cpp)

add_library(mssc ${MSSC_SOURCE_FILES})