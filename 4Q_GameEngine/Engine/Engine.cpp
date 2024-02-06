#include "pch.h"
#include "Engine.h"


#include <imgui.h>

#include <directxtk/SimpleMath.h>


#include "BoxCollider.h"
#include "CameraScript.h"
#include "CameraSystem.h"
#include "Debug.h"
#include "DebugSystem.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Movement.h"
#include "MovementSystem.h"
#include "RenderManager.h"
#include "Script.h"
#include "WorldManager.h"
#include "RenderSystem.h"
#include "SampleScript.h"
#include "ScriptSystem.h"
#include "SoundManager.h"
#include "SpriteSystem.h"
#include "StaticMesh.h"
#include "TransformSystem.h"
#include "imgui.h"

#define ENGINE_DEBUG

#ifdef ENGINE_DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int newWidth = 0;
int newHeight = 0;

Engine::Engine(HINSTANCE hInstance)
	: m_hWnd()
	, m_hInstance(hInstance)
	, m_Msg()
	, m_szTitle{ L"4Q_Engine" }
	, m_szWindowClass{ L"DefaultWindowClass" }
	, m_ClientWidth(1024)
	, m_ClientHeight(768)
{
	m_Wcex.hInstance = hInstance;
	m_Wcex.cbSize = sizeof(WNDCLASSEX);
	m_Wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_Wcex.lpfnWndProc = WndProc;
	m_Wcex.cbClsExtra = 0;
	m_Wcex.cbWndExtra = 0;
	m_Wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_Wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	m_Wcex.lpszClassName = m_szWindowClass;
	m_bIsRunning = true;

}

Engine::~Engine()
{
}

bool Engine::Initialize(const UINT width, const UINT height)
{
	// 윈도우 초기화
	m_ClientWidth = width;
	m_ClientHeight = height;

	RegisterClassExW(&m_Wcex);

	RECT rcClient = { 0,0,static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		100, 100, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top
		, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

#ifdef NDEBUG
	int speed = 10;
	SystemParametersInfo(SPI_SETMOUSESPEED, 0, (void*)speed, SPIF_SENDCHANGE);
	ShowCursor(FALSE);
#endif


	// 매니저 초기화
	RenderManager::GetInstance()->Initialize(&m_hWnd, width, height);
	TimeManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->Initialize();

	WorldManager::GetInstance()->ChangeWorld(World::CreateWorld(L"../Test/TestScene1.json"));
	EntitySystem* scriptSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new ScriptSystem());
	EntitySystem* movementSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new MovementSystem());
	EntitySystem* transformSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new TransformSystem());
	EntitySystem* debugSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new DebugSystem());
	EntitySystem* cameraSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new CameraSystem());
	EntitySystem* renderSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new RenderSystem());
	
	Entity* ent = WorldManager::GetInstance()->GetCurrentWorld()->create();
	ent->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 10.f,10.f,10.f });
	ent->Assign<Debug>();
	ent->Assign<Camera>();
	ent->Assign<CameraScript>(ent);
	ent->Assign<Movement>();

	//bool b = ent->has<Script>();
	Entity* ent1 = WorldManager::GetInstance()->GetCurrentWorld()->create();
	ent1->Assign<StaticMesh>("FBXLoad_Test/fbx/plane.fbx");
	ent1->Assign<Transform>(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), Vector3D{ 100.f,100.f,100.f });

	Entity* ent2 = WorldManager::GetInstance()->GetCurrentWorld()->create();
	ent2->Assign<StaticMesh>("FBXLoad_Test/fbx/zeldaPosed001.fbx");
	ent2->Assign<Transform>(Vector3D(100.f, 0.f, 0.f));


	SoundManager::GetInstance()->CreateSound("better-day-186374.mp3", true);	
	SoundManager::GetInstance()->PlayBackSound("better-day-186374.mp3");
  
	RenderManager::GetInstance()->AddSprite(1, "../Resource/UI/image.jpg", { 0,0 }, 0);
	RenderManager::GetInstance()->AddSprite(2, "../Resource/UI/image2.jpg", { 50,0 }, 1);

	 

	return true;
}

void Engine::Run()
{
	while(m_bIsRunning)
	{
		if(PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT)
				break;

			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}
		else
		{
			if (m_ClientHeight != 0 && m_ClientWidth != 0)
			{
				m_ClientHeight = newHeight;
				m_ClientWidth = newWidth;
			}

			Update();
			Render();
		}
	}
	RenderManager::GetInstance()->UnInitialize();
}

void Engine::Update()
{
	TimeManager::GetInstance()->Update();
	SoundManager::GetInstance()->Update();
	const float deltaTime = TimeManager::GetInstance()->GetDeltaTime();
	WorldManager::GetInstance()->Update(deltaTime);
	InputManager::GetInstance()->Update(deltaTime);
	RenderManager::GetInstance()->Update();
}

void Engine::Render()
{
	RenderManager::GetInstance()->RenderBegin();
	RenderManager::GetInstance()->Render();
	RenderManager::GetInstance()->RenderEnd();
}

void Engine::Close()
{
	m_bIsRunning = false;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int resizeWidth = 0;
int resizeHeight = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;	switch (message)
	{
	case WM_SIZE:
		newWidth = LOWORD(lParam);
		newHeight = HIWORD(lParam);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
