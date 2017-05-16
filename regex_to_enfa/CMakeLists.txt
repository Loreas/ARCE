cmake_minimum_required(VERSION 3.5)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(REGEX_TO_ENFA_SOURCE_FILES
        regex_to_enfa.h
        regex_to_enfa.cpp)

add_library(regex_to_enfa ${REGEX_TO_ENFA_SOURCE_FILES})
