#include "Header.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("DirectXBase");

// DirectX 인터페이스
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pDeviceContext = nullptr;
IDXGISwapChain* pSwapChain = nullptr;
ID3D11RenderTargetView* pRenderTargetView = nullptr;

// 셰이더와 입력 레이아웃
ID3D11VertexShader* pVertexShader = nullptr;
ID3D11PixelShader* pPixelShader = nullptr;
ID3D11InputLayout* pVertexLayout = nullptr;

// 버텍스 버퍼
ID3D11Buffer* pVertexBuffer = nullptr;

// 셰이더 코드
const char* vertexShaderSource = R"(
    float4 main(float4 Pos : POSITION ) : SV_POSITION
    {
        return Pos;
    }
)";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASS WndClass;
    g_hInst = hInstance;

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WndProc;
    WndClass.lpszClassName = lpszClass;
    WndClass.lpszMenuName = NULL;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL,
                        (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);

    // 디바이스, 디바이스 컨텍스트, 스왑체인 생성
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = WIDTH;
    swapChainDesc.BufferDesc.Height = HEIGHT;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = true;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, nullptr,
        &pDeviceContext);

    if (FAILED(hr)) return -1;

    // 렌더 타겟 뷰 생성
    ID3D11Texture2D* pBackBuffer = nullptr;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                          reinterpret_cast<LPVOID*>(&pRenderTargetView));

    pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
    pBackBuffer->Release();

    pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);

    // 뷰포트 생성
    D3D11_VIEWPORT vp{};
    vp.Width = WIDTH;
    vp.Height = HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    pDeviceContext->RSSetViewports(1, &vp);

    // 셰이더 컴파일 및 생성
    ID3DBlob* pVSBlob = nullptr;
    D3DCompile()

        while (GetMessage(&Message, NULL, 0, 0))
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            return 0;
    }

    return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Render()
{
    if (pDevice == nullptr) PostQuitMessage(-1);

    pDevice
}