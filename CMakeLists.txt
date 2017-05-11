cmake_minimum_required(VERSION 3.5)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

add_subdirectory(MSSC)
add_subdirectory(FA)
add_subdirectory(regex_to_enfa)
add_subdirectory(TFA)

add_executable(ARCE ${SOURCE_FILES})
target_link_libraries(ARCE MSSC FA regex_to_enfa TFA)