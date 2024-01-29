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

protected:
	HWND m_hWnd;
	UINT m_ClientWidth;
	UINT m_ClientHeight;

private:
	HINSTANCE m_hInstance;
	MSG m_Msg;
	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	WNDCLASSEXW m_Wcex;


	//std::unique_ptr<TimeManager> m_Time = nullptr;
	InputManager* m_Input = nullptr;
	RenderManager* m_Render = nullptr;

private:
	bool m_bIsRunning = false;
public:
	virtual bool Initialize(UINT width, UINT height);
	virtual void Run();
	virtual void Update();
	virtual void Render();

	void Close();
};

