#include "D3dApp.h"


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

        outs << mMainWndCaption << L"    "
        << L"FPS : " << fps <<  L"    "
        << L"Frame Time : " << mspf << L" (ms)";
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
    D3DApp* pThis = nullptr;

    /* WM_NCCREATE 메시지 처리 */
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        pThis = reinterpret_cast<D3DApp*>(pCreate->lpCreateParams); // CreateWindowExW의 마지막 인자로 전달된 this 포인터 획득
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)); // 윈도우 사용자 데이터에 객체 포인터 저장
    }
    else 
    {
        pThis = reinterpret_cast<D3DApp*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)); // 생성되어있는 사용자 데이터에서 객체 포인터 획득
    }

    /* 객체 포인터 유효시 호출 */
    if (pThis) return pThis->MsgProc(hWnd, msg, wParam, lParam);

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}