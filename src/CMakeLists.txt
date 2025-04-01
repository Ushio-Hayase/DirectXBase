if(NOT WIN32)
    message(FATAL_ERROR "This project requires Windows and Windows SDK")
endif()


add_executable(DirectXBase WIN32 main.cpp D3DAppChap4.cpp D3dApp.cpp GameTimer.cpp)

target_include_directories(DirectXBase PUBLIC ../Include)

target_link_libraries(DirectXBase user32 gdi32 d3d11 dxgi d3dcompiler)

