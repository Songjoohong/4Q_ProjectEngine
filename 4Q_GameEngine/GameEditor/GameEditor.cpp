#include "pch.h"
#include "GameEditor.h"
#include "../D3D_Graphics/D3D_Graphics.h"
#include "../Engine/ECS.h"
#include "../Engine/WorldManager.h"

#include "ImGuizmo.h"
// Component Headers
#include "../Engine/Transform.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/Camera.h"
#include "../Engine/Light.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Movement.h"
#include "../Engine/StaticMesh.h"
#include "../Engine/Debug.h"
#include "../Engine/TransformSystem.h"
#include "../Engine/CameraSystem.h"
#include "../Engine/MovementSystem.h"
#include "../Engine/Sound.h"
#include "../Engine/RenderManager.h"
#include "../Engine/ScriptSystem.h"
#include "../Engine/Sprite2D.h"
#include "../Engine/CameraScript.h"
#include "../Engine/RenderSystem.h"

#include "Prefab.h"
#include "../D3D_Graphics/RenderTextureClass.h"


using json = nlohmann::json;
namespace ECS { class Entity; }

GameEditor::GameEditor(HINSTANCE hInstance)
	:Engine(hInstance)
{
}

GameEditor::~GameEditor()
{
	m_EditorWorld->DestroyWorld();
	delete m_Renderer;
	ShutDownImGui();
}

bool GameEditor::Initialize(UINT width, UINT height)
{
	__super::Initialize(width, height);

	m_Renderer = Renderer::Instance;
	
	//m_EditorWorld = ECS::World::CreateWorld("TestScene1.json");

	m_EditorWorld = WorldManager::GetInstance()->GetCurrentWorld();		// test
	m_SceneHierarchyPanel.SetContext(m_EditorWorld, m_PrefabManager);			// test

	//m_EditorWorld->registerSystem(new RenderSystem);
	//m_EditorWorld->registerSystem(new TransformSystem);

	//WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);



	m_PrefabManager = std::make_shared<PrefabManager>(m_EditorWorld);

	m_ContentsBrowserPanel.SetContext(m_EditorWorld);
	m_SceneHierarchyPanel.SetContext(m_EditorWorld, m_PrefabManager);
	m_ContentsBrowserPanel.Initialize();
	if (!InitImGui())
	{
		return false;
	}
	return true;
}

void GameEditor::Update()
{
	__super::Update();
}

void GameEditor::Render()
{
	RenderManager::GetInstance()->RenderBegin();
	RenderManager::GetInstance()->Render();
	RenderImGui();
	RenderManager::GetInstance()->RenderEnd();
}


bool GameEditor::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImFontConfig imguiFontConfig;
	imguiFontConfig.MergeMode = false;
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/font/Roboto-SemiMedium.ttf", 15.0f, &imguiFontConfig, io.Fonts->GetGlyphRangesDefault());
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetDarkThemeColors();	// 유니티 한스푼 첨가.

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_Renderer->m_pDevice.Get(), m_Renderer->m_pDeviceContext.Get());

	return true;
}

void GameEditor::BeginRenderImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void GameEditor::RenderImGui()
{
	BeginRenderImGui();

	// Note : Swtich this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		static bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		//// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		//	// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		//if (m_show_another_window)
		//	ImGui::ShowDemoWindow(&m_show_another_window);
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;								// 패널 좌우 간격 최소치를 정한다. -> 사용자가 패널을 줄여서 정보가 보이지 않는 것을 방지.
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					LoadWorld("TestScene1");	// Test

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save World"))
				{
					SaveWorld("TestScene1.json");
				}

				if (ImGui::MenuItem("Load World"))
				{
					LoadWorld("TestScene1.json");
				}

				//if (ImGui::MenuItem("Test Dialog"))
				//{
				//}

				if (ImGui::MenuItem("Exit"))
				{
					Close();
				}

				if (ImGui::MenuItem("ClearPrefabFile"))
				{
					m_PrefabManager->DeleteAllDataInJsonFile("Prefab.json");
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		m_SceneHierarchyPanel.RenderImGui();
		m_ContentsBrowserPanel.RenderImGui();

		static bool show = true;
		ImGui::ShowDemoWindow();

		ImGui::End();

		/* Viewport ------------------------ */
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });	// 패딩 제거
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ID3D11ShaderResourceView* myViewportTexture = RenderManager::GetInstance()->GetRender()->m_RenderTexture->GetShaderResourceView();
		ImGui::Image((void*)myViewportTexture, ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
		Entity* selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		// CameraEntity가 나와야 다시 할 수 있을듯?
		// Projection행렬 필요
		//if (selectedEntity)
		//{
		//	ImGuizmo::SetOrthographic(false);
		//	ImGuizmo::SetDrawlist();
		//	float windowWidth = (float)ImGui::GetWindowWidth();
		//	float windowHeight = (float)ImGui::GetWindowHeight();
		//	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		//	// Camera
		//	const auto& camera = m_Camera->get<Transform>().get();
		//	DirectX::XMMATRIX camTranslation = DirectX::XMMatrixTranslationFromVector(camera.m_Position.ConvertToVector3());
		//	DirectX::XMMATRIX camRotation = DirectX::XMMatrixRotationQuaternion(camera.m_Rotation.ConvertToVector3());
		//	DirectX::XMMATRIX camScale = DirectX::XMMatrixScalingFromVector(camera.m_Scale.ConvertToVector3());
		//	auto cameratransformMatrix = camScale * camRotation * camTranslation;

		//	// Entity Transform
		//	auto& tc = selectedEntity->get<Transform>().get();
		//	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslationFromVector(tc.m_Position.ConvertToVector3());
		//	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(tc.m_Rotation.ConvertToVector3());
		//	DirectX::XMMATRIX scale = DirectX::XMMatrixScalingFromVector(tc.m_Scale.ConvertToVector3());

		//	auto transformMatrix = scale * rotation * translation;
		//	
		//	//ImGuizmo::Manipulate(cameratransformMatrix, )
		//}

		ImGui::End();
		ImGui::PopStyleVar();

	}
	else
	{
		static bool show = true;
		ImGui::ShowDemoWindow();
	}

	EndRenderImGui();
}

void GameEditor::EndRenderImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(m_ClientWidth, m_ClientHeight);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		//glfwMakeContextCurrent(backup_current_context);
	}
}

void GameEditor::ShutDownImGui()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GameEditor::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}


// jsonFile 이름 넘기기
void GameEditor::SaveWorld(const std::string& _filename)
{
	std::string fullPath = basePath + _filename;

	std::ofstream outputFile(fullPath);

	json worldData;
	for (const auto& entity : m_EditorWorld->GetEntities())
	{
		SaveComponents<EntityIdentifier>(entity, worldData);
		SaveComponents<Transform>(entity, worldData);
		SaveComponents<StaticMesh>(entity, worldData);
		SaveComponents<BoxCollider>(entity, worldData);
		SaveComponents<Camera>(entity, worldData);
		SaveComponents<Light>(entity, worldData);
		SaveComponents<Movement>(entity, worldData);
		SaveComponents<Debug>(entity, worldData);
		SaveComponents<Sound>(entity, worldData);
		SaveComponents<Sprite2D>(entity, worldData);
		SaveComponents<CameraScript>(entity, worldData);
	}

	outputFile << std::setw(4) << worldData << std::endl;

	outputFile.close();

}

void GameEditor::LoadWorld(const std::string& _filename)
{
	// 월드 생성
	m_EditorWorld = ECS::World::CreateWorld(_filename);

	std::string fullPath = basePath + _filename;

	// Deserialize
	std::ifstream inputFile(fullPath);
	json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	for (const auto& entity : jsonObject["WorldEntities"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			// Entity 생성 후 정보 push
			Entity* myEntity = m_EditorWorld->create();

			for (const auto& component : it.value())
			{
				std::string componentName = component.begin().key();

				if (componentName == "EntityIdentifier")
				{
					AssignComponents<EntityIdentifier>(myEntity, component["EntityIdentifier"][0]);
				}
				else if (componentName == "Transform")
				{
					AssignComponents<Transform>(myEntity, component["Transform"][0]);
				}
				else if (componentName == "BoxCollider")
				{
					AssignComponents<BoxCollider>(myEntity, component["BoxCollider"][0]);
				}

				else if (componentName == "Camera")
				{
					AssignComponents<Camera>(myEntity, component["Camera"][0]);
				}

				else if (componentName == "Light")
				{
					AssignComponents<Light>(myEntity, component["Light"][0]);
				}

				else if (componentName == "Movement")
				{
					AssignComponents<Movement>(myEntity, component["Movement"][0]);
				}

				else if (componentName == "StaticMesh")
				{
					AssignComponents<StaticMesh>(myEntity, component["StaticMesh"][0]);
				}
				else if (componentName == "Debug")
				{
					AssignComponents<Debug>(myEntity, component["Debug"][0]);
				}
				else if (componentName == "Sound")
				{
					AssignComponents<Sound>(myEntity, component["Sound"][0]);
				}
				else if (componentName == "CameraScript")
				{
					AssignComponents<CameraScript>(myEntity, component["CameraScript"][0]);
				}
			}
		}
	}


	//부모자식 관계 설정
	for (const auto& childEntity : m_EditorWorld->GetEntities())
	{	
		for (const auto& parentEntity : m_EditorWorld->GetEntities())
		{
			if (childEntity->get<EntityIdentifier>().get().m_HasParent == true)
			{
				if (childEntity->get<EntityIdentifier>().get().m_ParentEntityId == parentEntity->getEntityId())
				{
					SetParent(childEntity, parentEntity);
				}
			}
		}
	}

	// HierarchyPanel에 등록
	m_SceneHierarchyPanel.SetContext(m_EditorWorld, m_PrefabManager);
	m_ContentsBrowserPanel.SetContext(m_EditorWorld);
	WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);
}

void GameEditor::NewScene()
{
	m_EditorWorld = ECS::World::CreateWorld("TestScene1.json");
	m_EditorWorld->registerSystem(new RenderSystem);
	m_EditorWorld->registerSystem(new TransformSystem);
	m_EditorWorld->registerSystem(new MovementSystem);
	m_EditorWorld->registerSystem(new CameraSystem);
	m_EditorWorld->registerSystem(new ScriptSystem);

	m_Camera = m_EditorWorld->create();
	m_Box = m_EditorWorld->create();
	m_Pot = m_EditorWorld->create();
	m_Wall = m_EditorWorld->create();

	Vector3D pos1 = { 1.0f, 3.0f, 5.0f };
	Vector3D pos2 = { 10.0f, 30.0f, 50.0f };
	Vector3D pos3 = { 100.0f, 300.0f, 500.0f };

	m_Camera->Assign<EntityIdentifier>(m_Camera->getEntityId(), "Camera");
	m_Camera->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 10.f,10.f,10.f });
	//m_Camera->Assign<Camera>();
	//m_Camera->Assign<CameraScript>(m_Camera);
	//m_Camera->Assign<Movement>();
	//m_Camera->Assign<Debug>();

	m_Box->Assign<EntityIdentifier>(m_Box->getEntityId(), "Box");
	m_Pot->Assign<EntityIdentifier>(m_Pot->getEntityId(), "Pot");

	SetParent(m_Pot, m_Box);

	m_Wall->Assign<EntityIdentifier>(m_Wall->getEntityId(), "Wall");

	SetParent(m_Wall, m_Pot);

	m_Box->Assign<Transform>(pos1);
	m_Pot->Assign<Transform>(pos2);

	m_Wall->Assign<StaticMesh>("FBXLoad_Test/fbx/box.fbx");
	m_Wall->Assign<Transform>(Vector3D(0.f, 0.f, -50.f), Vector3D(0.f, 0.f, 0.f), Vector3D(100.f, 100.f, 100.f));
	m_Wall->Assign<Debug>();

	m_Camera->Assign<Light>();

	m_SceneHierarchyPanel.SetContext(m_EditorWorld, m_PrefabManager);
	m_ContentsBrowserPanel.SetContext(m_EditorWorld);
	WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);
}

void GameEditor::SetParent(ECS::Entity* child, ECS::Entity* parent)
{
	child->get<EntityIdentifier>().get().m_ParentEntityId = parent->getEntityId();
	child->get<EntityIdentifier>().get().m_HasParent = true;

	parent->addChild(child);
}
