#pragma once

#include "D3dApp.h"

#include <dxgi.h>

class Chap4 : public D3DApp
{
    public:
    Chap4(HINSTANCE hInstance);
    ~Chap4();

    bool Init() override;
    void OnResize() override;
    void UpdateScene(float dt) override;
    void DrawScene() override;
    LRESULT CALLBACK MsgProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM LPARAM) override;

    protected:
    bool InitMainWindow() override;
    bool InitDirect3D() override;
};
