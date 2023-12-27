#pragma once
#include <Windows.h>

constexpr int MAX_LOADSTRING = 100;

class Engine
{
public:
	//static Engine* m_pInstance;
	//static HWND m_hWnd;
	Engine(HINSTANCE hInstance);
	virtual ~Engine();

private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	MSG m_Msg;
	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	WNDCLASSEXW m_Wcex;

	UINT m_ClientWidth;
	UINT m_ClientHeight;

public:
	virtual bool Initialize(UINT Width, UINT Height);
	virtual void Run();
	virtual void Update();
	virtual void Render();

	
};

