cmake_minimum_required(VERSION 3.7)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        FA/DFA.cpp
        FA/DFA.h
        FA/eNFA.cpp
        FA/eNFA.h
        FA/FA.cpp
        FA/FA.h
        FA/NFA.cpp
        FA/NFA.h)

add_executable(ARCE ${SOURCE_FILES})