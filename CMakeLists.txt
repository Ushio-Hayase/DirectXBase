cmake_minimum_required(VERSION 3.20)
project(DirectXBase)

set(OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/build)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)



set_target_properties(DirectXBase PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})