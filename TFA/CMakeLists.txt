cmake_minimum_required(VERSION 3.7)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(TFA_SOURCE_FILES
        tfa.cpp)

add_library(TFA ${TFA_SOURCE_FILES})