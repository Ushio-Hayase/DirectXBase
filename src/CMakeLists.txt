if(NOT WIN32)
    message(FATAL_ERROR "This project requires Windows and Windows SDK")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



add_executable(DirectXBase WIN32 main.cpp D3DAppChap4.cpp D3dApp.cpp GameTimer.cpp)

target_link_libraries(DirectXBase user32 gdi32 d3d11 dxgi d3dcompiler)
