cmake_minimum_required(VERSION 3.7)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(REGEX_TO_ENFA_SOURCE_FILES
        *.cpp
        *.h)

add_library(regex_to_enfa REGEX_TO_ENFA_SOURCE_FILES)