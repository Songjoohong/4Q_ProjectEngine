#pragma once
#include <Windows.h>

class InputManager;
class TimeManager;
class RenderManager;
constexpr int MAX_LOADSTRING = 100;

class Engine
{
public:
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

	std::unique_ptr<TimeManager> m_Time = nullptr;
	InputManager* m_Input = nullptr;
	RenderManager* m_Render = nullptr;
public:
	virtual bool Initialize(UINT width, UINT height);
	virtual void Run();
	virtual void Update();
	virtual void Render();


	
};

