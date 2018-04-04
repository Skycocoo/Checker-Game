# Created by Yuxi Luo, April 3 2018
# Referencing CMakeFiles: https://github.com/Liby99/Rotamina/blob/master/CMakeLists.txt


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

# compile all the test files
file(GLOB test_sources src/main.cpp)

foreach(test_source ${test_sources})
    string(REPLACE ".cpp" "" temp_path ${test_source})
    string(REPLACE "${CMAKE_CURRENT_LIST_DIR}/" "" temp_path ${temp_path})
    get_filename_component(temp_dir ${temp_path} DIRECTORY)
    get_filename_component(temp_name ${temp_path} NAME)
    add_executable(${temp_name} ${test_source})
    set_target_properties(${temp_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${temp_dir})
    target_link_libraries(${temp_name} checker)
    include("${temp_dir}/CMakeLists.txt" OPTIONAL)
endforeach(test_source ${test_sources})
