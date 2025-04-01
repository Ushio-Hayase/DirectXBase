cmake_minimum_required(VERSION 3.20)
project(DirectXBase)

add_subdirectory(src)

set(OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/build)


target_include_directories(DirectXBase PUBLIC Include)

set_target_properties(DirectXBase PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})