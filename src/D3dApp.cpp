#include "D3dApp.h"

D3DApp::D3DApp(HINSTANCE hInst)
    : mhAppInst(hInst), mAppPaused(false), mMinimized(true), mMaximized(false), mResizing(false), m4xMsaaQuality(4),
      mClientHeight(400), mClientWidth(600)
{
}

D3DApp::~D3DApp()
{
    md3dDevice->Release();
    md3dDeviceContext->Release();
    mRenderTargetView->Release();
    mSwapChain->Release();
    mDepthStencilView->Release();
    mDepthStencilBuffer->Release();
   
    
}

/// @return 앱의 인스턴스 핸들 반환
HINSTANCE D3DApp::AppInst() const
{
    return mhAppInst;
}

/// @return 메인 윈도우 핸들 반환
HWND D3DApp::MainWnd() const
{
    return mhMainWnd;
}

/// @brief 종횡비 계산
/// @return 계산한 종횡비 반환
float D3DApp::AspectRatio() const
{
    return static_cast<float>(mClientWidth) / mClientHeight;
}

int D3DApp::Run()
{
    MSG msg = {0};

    mTimer.Reset();

    ShowWindow(mhMainWnd, SW_SHOWDEFAULT);
    UpdateWindow(mhMainWnd);

    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else
        {
            mTimer.Tick();

            if (!mAppPaused)
            {
                CalculateFrameStats();
                UpdateScene(mTimer.DeltaTime());
                DrawScene();
            }
            else
            {
                Sleep(100);
            }
        }
    }
    return 0;
}

/// @brief 평균 FPS 계산 및 프레임 시간 계산
void D3DApp::CalculateFrameStats()
{
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;

    frameCnt++;

    /* 1초 동안의 평균 프레임수 계산 */
    if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
    {
        float fps = static_cast<float>(frameCnt); // fps = frameCnt / 1
        float mspf = 1000.0f / fps;

        std::wostringstream outs;
        outs.precision(6);

        outs << mMainWndCaption << L"    " << L"FPS : " << fps << L"    " << L"Frame Time : " << mspf << L" (ms)";
        SetWindowTextW(mhMainWnd, outs.str().c_str());

        /* 다음 프레임을 위해 초기화 */
        frameCnt = 0;
        timeElapsed += 1.0f;
    }
}

/// @brief 메시지 프로시저 래퍼 함수
/// @param[in] hWnd 윈도우의 핸들
/// @param[in] msg 수신된 메시지 종류
/// @param[in] wParam 메시지 기타 정보
/// @param[in] lParam 메시지 기타 정보
/// @return 실행 결과 코드 반환
LRESULT CALLBACK D3DApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    D3DApp *pThis = nullptr;

    /* WM_NCCREATE 메시지 처리 */
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCTW *pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
        pThis = reinterpret_cast<D3DApp *>(
            pCreate->lpCreateParams); // CreateWindowExW의 마지막 인자로 전달된 this 포인터 획득
        SetWindowLongPtrW(hWnd, GWLP_USERDATA,
                          reinterpret_cast<LONG_PTR>(pThis)); // 윈도우 사용자 데이터에 객체 포인터 저장
    }
    else
    {
        pThis = reinterpret_cast<D3DApp *>(
            GetWindowLongPtrW(hWnd, GWLP_USERDATA)); // 생성되어있는 사용자 데이터에서 객체 포인터 획득
    }

    /* 객체 포인터 유효시 호출 */
    if (pThis)
        return pThis->MsgProcImpl(hWnd, msg, wParam, lParam);

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

/// @brief 멤버 변수들 초기화
/// @return 성공 여부 반환
bool D3DApp::Init()
{
    if (!InitMainWindow() || !InitDirect3D())
    {
        return false;
    }

    return true;
}

/// @brief 메인 윈도우 WINAPI 초기화
/// @return 성공 여부 반환
bool D3DApp::InitMainWindow()
{
    WNDCLASSEXW wcex; // 윈도우 클래스 정보 구조체

    wcex.cbSize = sizeof(WNDCLASSEXW);                        // 구조체 크기
    wcex.style = CS_HREDRAW | CS_VREDRAW;                     // 스타일 (크기 변경시 다시 그리기)
    wcex.lpfnWndProc = MsgProc;                               // 윈도우 프로시저 함수 포인터
    wcex.cbClsExtra = 0;                                      // 클래스 익스트라 데이터 (사용 안함)
    wcex.cbWndExtra = 0;                                      // 윈도우 익스트라 데이터 (사용 안함)
    wcex.hInstance = mhAppInst;                               // 인스턴스 핸들
    wcex.hIcon = LoadIcon(mhAppInst, IDI_APPLICATION);        // 기본 아이콘
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);            // 기본 커서 (화살표)
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);          // 배경 브러시 (기본 윈도우 배경색)
    wcex.lpszMenuName = nullptr;                              // 메뉴 이름 (메뉴 없음)
    wcex.lpszClassName = this->mMainWndCaption.c_str();       // 윈도우 클래스 이름 (유니코드)
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); // 작은 아이콘

    if (!RegisterClassExW(&wcex))
    {
        return false;
    }

    mhMainWnd = CreateWindowExW(0, mMainWndCaption.c_str(), mMainWndCaption.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, mhAppInst, nullptr);

    if (mhMainWnd == nullptr)
    {
        MessageBoxW(mhMainWnd, L"CreateWindowExW failed!", L"ERROR", MB_ICONERROR);
        return false;
    }

    return true;
}

/// @brief Direct3D 요소 초기화
/// @return 성공 여부 반환
bool D3DApp::InitDirect3D()
{
    /* 스왑 체인 정의 구조체 설정 */
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Width = mClientWidth;
    swapChainDesc.BufferDesc.Height = mClientHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    if (mEnable4XMsaa)
    {
        swapChainDesc.SampleDesc.Count = 4;
        swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
    }

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = mhMainWnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D11CreateDeviceAndSwapChain(nullptr, md3dDriverType, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
                                  &mSwapChain, &md3dDevice, 0,
                                  &md3dDeviceContext); // 스왑 체인과 디바이스, 디바이스 컨텍스트 생성

    /* 렌더 타겟팅 뷰 생성 */
    ID3D11Texture2D *backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));
    md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
    backBuffer->Release();

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if (mEnable4XMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    if (FAILED(md3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer)))
    {
        return false;
    }
    if (FAILED(md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView)))
    {
        return false;
    }

    md3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    mScreenViewport.Height = mClientHeight;
    mScreenViewport.Width = mClientWidth;
    mScreenViewport.TopLeftX = 0.0f;
    mScreenViewport.TopLeftY = 0.0f;
    mScreenViewport.MaxDepth = 1.0f;
    mScreenViewport.MinDepth = 0.0f;

    md3dDeviceContext->RSSetViewports(1, &mScreenViewport);

    return true;
}

void D3DApp::OnResize()
{
    mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    mDepthStencilView->Release();
    mDepthStencilBuffer->Release();
    mRenderTargetView->Release();

    ID3D11Texture2D *backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer));
    md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
    backBuffer->Release();

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = mClientWidth;
    depthStencilDesc.Height = mClientHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if (mEnable4XMsaa)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);

    md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

    md3dDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void D3DApp::OnMouseDown(WPARAM wParam, int x, int y)
{
}

void D3DApp::OnMouseMove(WPARAM wParam, int x, int y)
{
}

void D3DApp::OnMouseUp(WPARAM wParam, int x, int y)
{
}