cmake_minimum_required(VERSION 3.20)
project(DirectXBase)

if(NOT WIN32)
    message(FATAL_ERROR "This project requires Windows and Windows SDK")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(DirectXBase "../Include")

add_executable(DirectXBase main.cpp)

target_link_libraries(DirectXBase PRIVATE user32 gdi32 d3d11 dxgi d3dcompiler)


