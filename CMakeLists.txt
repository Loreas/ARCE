cmake_minimum_required(VERSION 3.5)
project(ARCE)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp)

add_subdirectory(src/algos)
add_subdirectory(src/automata)
add_subdirectory(src/bot_ARCE)

add_executable(ARCE ${SOURCE_FILES})
target_link_libraries(ARCE algos automata bot_ARCE)
