﻿#include "pch.h"
#include "Engine.h"


#include <codecvt>
#include <imgui.h>
#include <string.h>
#include <directxtk/SimpleMath.h>


#include "BoxCollider.h"
#include "POVCameraScript.h"
#include "FreeCameraScript.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "Debug.h"
#include "DebugSystem.h"
#include "DynamicText.h"
#include "DynamicTextScript.h"
#include "EntityIdentifier.h"
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
#include "UISystem.h"
#include "PhysicsManager.h"
#include "PlayerScript.h"
#include "RigidBody.h"
#include "TestUIScript.h"
#include "UI.h"
#include "PlayerInformation.h"
#include "EntityIdentifier.h"
#include "Space.h"
#include "SpaceSystem.h"
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
	AdjustWindowRect(&rcClient, WS_POPUP, FALSE);

	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_POPUP,
		0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top
		, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	// 매니저 초기화
	RenderManager::GetInstance()->Initialize(&m_hWnd, width, height);
	TimeManager::GetInstance()->Initialize();
	SoundManager::GetInstance()->Initialize();
	PhysicsManager::GetInstance()->Initialize();
	InputManager::GetInstance()->Initialize(m_ClientWidth, m_ClientHeight);

	//WorldManager::GetInstance()->ChangeWorld(World::CreateWorld("../Test/TestScene1.json"));
	//EntitySystem* scriptSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new ScriptSystem());
	//EntitySystem* movementSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new MovementSystem());
	//EntitySystem* collisionSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new CollisionSystem());
	//EntitySystem* transformSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new TransformSystem());
	//EntitySystem* debugSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new DebugSystem());
	//EntitySystem* cameraSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new CameraSystem());
	//EntitySystem* renderSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new RenderSystem());
	//EntitySystem* spriteSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new SpriteSystem());
	//EntitySystem* UISystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new class UISystem);
	//EntitySystem* spaceSystem = WorldManager::GetInstance()->GetCurrentWorld()->registerSystem(new SpaceSystem());


	////Free Camera
	//Entity* ent = WorldManager::GetInstance()->GetCurrentWorld()->create();
	//ent->Assign<EntityIdentifier>(ent->getEntityId(), "Camera");
	//ent->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 0.f,0.f,0.f });
	//ent->Assign<Debug>();
	//ent->Assign<Camera>();
	//ent->Assign<FreeCameraScript>(ent);
	//ent->Assign<Movement>();

	//Entity* ent1 = WorldManager::GetInstance()->GetCurrentWorld()->create();
	//ent1->Assign<EntityIdentifier>(ent1->getEntityId(), "Ground");
	//ent1->Assign<StaticMesh>("FBXLoad_Test/fbx/floor2_low.fbx");
	//ent1->Assign<Transform>(Vector3D(0.f, 0.f, 0.f), Vector3D(0.f, 0.f, 0.f), Vector3D{ 1.f,1.f,1.f });
	//ent1->Assign<BoxCollider>(ColliderType::STATIC, CollisionMask::GROUND, Vector3D{ 1000.f,1.f,1000.f });



	//vector<ExitInfo> exit;
	//exit.push_back({ 1, Vector3D{ 100.f,100.f,100.f } });
	//Entity* ent2 = WorldManager::GetInstance()->GetCurrentWorld()->create();
	//ent2->Assign<EntityIdentifier>(ent2->getEntityId(), "Player");
	//ent2->Assign<Transform>(Vector3D(0.f, 100.f, 0.f));
	//
	//ent2->Assign<Space>(1, exit);
	//ent2->Assign<Debug>();
	//ent2->Assign<PlayerScript>(ent2);
	//ent2->Assign<RigidBody>();
	//ent2->Assign<Movement>();

	//setlocale(LC_ALL, "Korean");
	//std::vector<std::wstring> wideStrings;
	//std::wstring text = L"안녕하세요";
	//std::wstring text1 = L"안녕히\n가세요";

	//wideStrings.push_back(text);
	//wideStrings.push_back(text1);

	//Entity* ent3 = WorldManager::GetInstance()->GetCurrentWorld()->create();

	//ent3->Assign<EntityIdentifier>(ent3->getEntityId(), "Zelda");
	//ent3->Assign<DynamicText>(wideStrings);
	//ent3->Assign<DynamicTextScript>(ent3);
	//ent3->Assign<Transform>(Vector3D(200.f, 100.f, 100.f), Vector3D(70, 10, 10));
	//ent3->Assign<StaticMesh>("FBXLoad_Test/fbx/ridgepole_001.fbx");
	//



	//Entity* ent5 = WorldManager::GetInstance()->GetCurrentWorld()->create();
	//ent5->Assign<Transform>(Vector3D(100.f, 100.f, 0.f));
	//ent5->Assign<EntityIdentifier>(ent5->getEntityId(), "ui");
	//ent5->Assign<UI>(100, 100);
	//ent5->Assign<Sprite2D>("../Resource/UI/image.jpg", 0, 100, 100);
	//ent5->Assign<TestUIScript>(ent5);
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
	const float deltaTime = TimeManager::GetInstance()->GetDeltaTime();
	InputManager::GetInstance()->Update(deltaTime);
	SoundManager::GetInstance()->Update();
	WorldManager::GetInstance()->Update(deltaTime);
	PhysicsManager::GetInstance()->Update(deltaTime);
	RenderManager::GetInstance()->Update();
}

void Engine::Render()
{
	RenderManager::GetInstance()->RenderBegin();
	RenderManager::GetInstance()->GameAppRender();
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
