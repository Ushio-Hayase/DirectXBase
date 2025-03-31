#include "D3DAppChap4.h"

Chap4::Chap4(HINSTANCE hInstance) : D3DApp(hInstance) {}

Chap4::~Chap4() {}

bool Chap4::Init()
{
    mMainWndCaption = L"Chap4";
    if (!this->InitMainWindow() || !this->InitDirect3D()) return false;

    return true;
}

void Chap4::OnResize()
{
    D3DApp::OnResize();
}

void Chap4::UpdateScene(float dt)
{

}

void Chap4::DrawScene()
{
    assert(md3dDeviceContext);
    assert(mSwapChain);

    const float black[4]{0.0f, 0.0f, 0.0f, 1.0f};

    /* 후면 버퍼 지우기 */
    md3dDeviceContext->ClearRenderTargetView(mRenderTargetView, black);

    /* 깊이 버퍼를 1.0으로 스텐실 버퍼를 0으로 지움 */
    md3dDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

    mSwapChain->Present(0, 0);
}


LRESULT CALLBACK Chap4::MsgProcImpl(HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam)
{
    switch (msg)
    {
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                mAppPaused = true;
                mTimer.Stop();
            }
            else
            {
                mAppPaused = false;
                mTimer.Start();
            }
            return 0;
        case WM_ENTERSIZEMOVE:
            mAppPaused = true;
            mResizing = true;
            mTimer.Stop();
            return 0;
        case WM_EXITSIZEMOVE:
            mAppPaused = false;
            mResizing = false;
            mTimer.Start();
            OnResize();
            return 0;
        case WM_MENUCHAR:
            return MAKELRESULT(0, MNC_CLOSE);
        case WM_GETMINMAXINFO:
            reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 200;
            reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 200;
            return 0;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
            OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        case WM_MOUSEMOVE:
            OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            return 0;
        case WM_DESTROY:         // 윈도우 파괴 메시지
            PostQuitMessage(0);  // 프로그램 종료 메시지 큐에 추가
            return 0;
        default:
            return DefWindowProcW(hWnd, msg, wParam,
                                  lParam);  // 기본 메시지 처리
    }
}


