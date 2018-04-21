# Created by Yuxi Luo, April 3 2018

cmake_minimum_required(VERSION 3.6)
project(Checker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14")

# setup include directory
include_directories(src/extern)

# compile all the cpp files and link to `checker` library
file(GLOB checker_sources src/intern/*.cpp src/extern/*.h)
set(checker ${checker_sources})
add_library(checker ${checker})

add_executable(main src/main.cpp)
target_link_libraries(main checker)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./)
