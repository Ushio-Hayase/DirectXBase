#pragma once

#include <Windows.h>
#include <d3d11.h>

#include <sstream>
#include <string>

#include "D3dUtil.h"
#include "GameTimer.h"

class D3DApp
{
  public:
    D3DApp(HINSTANCE hInstance);
    virtual ~D3DApp();

    HINSTANCE AppInst() const;
    HWND MainWnd() const;
    float AspectRatio() const;

    int Run();

    static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    virtual bool Init();
    virtual void OnResize();
    virtual void UpdateScene(float dt) = 0;
    virtual void DrawScene() = 0;

    virtual void OnMouseDown(WPARAM btnState, int x, int y);
    virtual void OnMouseUp(WPARAM btnState, int x, int y);
    virtual void OnMouseMove(WPARAM btnState, int x, int y);

  protected:
    bool InitMainWindow();
    bool InitDirect3D();
    virtual LRESULT MsgProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

    void CalculateFrameStats();

  protected:
    HINSTANCE mhAppInst; // 응용 프로그램 인스턴스 핸들
    HWND mhMainWnd;      // 주 창 핸들
    bool mAppPaused;     // 응용 프로그램 정지 여부
    bool mMinimized;     // 최소화 여부
    bool mMaximized;     // 최대화 여부
    bool mResizing;      // 크기 조정 테두리 사용중 여부
    UINT m4xMsaaQuality; // 4X MSAA의 품질 수준

    GameTimer mTimer; // 경과 시간과 전체 시간 변수

    /* DirectX 관련 변수 */
    ID3D11Device *md3dDevice;
    ID3D11DeviceContext *md3dDeviceContext;
    IDXGISwapChain *mSwapChain;
    ID3D11Texture2D *mDepthStencilBuffer;
    ID3D11RenderTargetView *mRenderTargetView;
    ID3D11DepthStencilView *mDepthStencilView;
    D3D11_VIEWPORT mScreenViewport;

    std::wstring mMainWndCaption; // 창의 제목

    D3D_DRIVER_TYPE
    md3dDriverType = D3D_DRIVER_TYPE_HARDWARE; // 구동기 종류. Default : D3D_DRIVER_TYPE_HARDWARE

    int mClientWidth;  // 창 너비
    int mClientHeight; // 창 높이

    bool mEnable4XMsaa; // 4X MSAA 활성화 여부
};