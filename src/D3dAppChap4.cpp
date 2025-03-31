#include "D3dAppChap4.h"

Chap4::Chap4(HINSTANCE hInstance) : D3DApp(hInstance) {}

Chap4::~Chap4() {}

bool Chap4::Init()
{
    if (!this->InitMainWindow() || !this->InitDirect3D()) return false;
}

bool Chap4::InitMainWindow()
{
    WNDCLASSEXW wcex;  // 윈도우 클래스 정보 구조체

    wcex.cbSize = sizeof(WNDCLASSEXW);     // 구조체 크기
    wcex.style = CS_HREDRAW | CS_VREDRAW;  // 스타일 (크기 변경시 다시 그리기)
    wcex.lpfnWndProc = MsgProc;            // 윈도우 프로시저 함수 포인터
    wcex.cbClsExtra = 0;                   // 클래스 익스트라 데이터 (사용 안함)
    wcex.cbWndExtra = 0;                   // 윈도우 익스트라 데이터 (사용 안함)
    wcex.hInstance = mhAppInst;            // 인스턴스 핸들
    wcex.hIcon = LoadIconW(mhAppInst, IDI_APPLICATION);  // 기본 아이콘
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);      // 기본 커서 (화살표)
    wcex.hbrBackground =
        (HBRUSH)(COLOR_WINDOW + 1);  // 배경 브러시 (기본 윈도우 배경색)
    wcex.lpszMenuName = nullptr;     // 메뉴 이름 (메뉴 없음)
    wcex.lpszClassName =
        this->mMainWndCaption.c_str();  // 윈도우 클래스 이름 (유니코드)
    wcex.hIconSm = LoadIconW(wcex.hInstance, IDI_APPLICATION);  // 작은 아이콘

    if(!RegisterClassExW(&wcex)) {return false;}

    mhMainWnd = CreateWindowExW(
        0, mMainWndCaption.c_str(), mMainWndCaption.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, nullptr, nullptr, mhAppInst, nullptr);

    if (!mhMainWnd)
    {
        MessageBoxW(mhMainWnd, L"CreateWindowExW failed!", L"ERROR",
                    MB_ICONERROR);
        return false;
    }

    
}

LRESULT CALLBACK Chap4::MsgProcImpl(HWND hWnd, UINT msg, WPARAM wParam,
                                    LPARAM lParam)
{
    switch (msg)
    {
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE)
            {
            }
        case WM_DESTROY:         // 윈도우 파괴 메시지
            PostQuitMessage(0);  // 프로그램 종료 메시지 큐에 추가
            return 0;
        default:
            return DefWindowProcW(hWnd, msg, wParam,
                                  lParam);  // 기본 메시지 처리
    }
}

bool Chap4::InitDirect3D()
{
    /* 스왑 체인 정의 구조체 설정정 */
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferDesc.Width = mClientWidth;
    swapChainDesc.BufferDesc.Height = mClientHeight;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering =
        DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

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

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                                  nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
                                  &mSwapChain, &md3dDevice, 0,
                                  &md3dDeviceContext); // 스왑 체인과 디바이스, 디바이스 컨텍스트 생성
    
    /* 렌더 타겟팅 뷰 생성 */
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
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
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality-1;
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

    if(!md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer)) {return false;}
    if(!md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView)) { return false;}

    md3dDeviceContext->OMSetRenderTargets(1,  &mRenderTargetView, mDepthStencilView);   
}
