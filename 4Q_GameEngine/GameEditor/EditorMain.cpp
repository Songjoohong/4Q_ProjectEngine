// 01_imgui.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "GameEditor.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	GameEditor Editor(hInstance);  // 생성자에서 아이콘,윈도우 이름만 바꾼다
	if (!Editor.Initialize(1920, 1080))
		return -1;
	Editor.Run();
	// Debug Memory Leak Check at end point
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return (int)1;
}