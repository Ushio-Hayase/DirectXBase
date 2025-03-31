#include "D3dApp.h"
#include "GameTimer.h"





// WinMain (윈도우 프로그램 진입점)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPWSTR lpCmdLine, int nCmdShow)
{

    

    // 윈도우 보여주기 및 업데이트
    ShowWindow(hWnd,
               nCmdShow);  // 윈도우 표시 (nCmdShow: WinMain의 표시 상태 인자)
    UpdateWindow(hWnd);    // 윈도우 업데이트 (다시 그리기)

    // 메시지 루프
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {  // 메시지 큐에서 메시지 가져오기
        TranslateMessage(
            &msg);  // 키보드 메시지 번역 (가상 키 코드 -> 문자 메시지)
        DispatchMessageW(&msg);  // 윈도우 프로시저로 메시지 전달
    }

    return (int)msg.wParam;  // 프로그램 종료 코드 반환 (PostQuitMessage의 인자)
}