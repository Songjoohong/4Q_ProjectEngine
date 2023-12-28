#include "pch.h"



#include "GameApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
    AllocConsole();
    FILE* pCout;
    FILE* pCin;
    FILE* pCerr;
    freopen_s(&pCin, "CONIN$", "r", stdin);
    freopen_s(&pCout, "CONOUT$", "w", stdout);
    freopen_s(&pCerr, "CONOUT$", "w", stderr);
#endif

    Engine Engine(hInstance);
    if (!Engine.Initialize(1280, 960))
        return -1;

    Engine.Run();

#ifdef _DEBUG
    FreeConsole();
#endif

    return 0;
}
