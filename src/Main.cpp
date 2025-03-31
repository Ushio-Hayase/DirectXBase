#include "D3DAppChap4.h"
#include "GameTimer.h"

// WinMain (윈도우 프로그램 진입점)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Chap4 app(hInstance);

    

    if (!app.Init())
        return 1;

    return app.Run();
}