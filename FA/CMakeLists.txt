cmake_minimum_required(VERSION 3.7)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(FA_SOURCE_FILES
        DFA.cpp
        DFA.h
        eNFA.cpp
        eNFA.h
        FA.cpp
        FA.h
        NFA.cpp
        NFA.h)

add_library(fa ${FA_SOURCE_FILES})