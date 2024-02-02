#include "pch.h"
#include "GameEditor.h"
#include "../D3D_Graphics/D3D_Graphics.h"
#include "../D3D_Graphics/RenderTextureClass.h"
#include "../Engine/ECS.h"
#include "../Engine/WorldManager.h"

// Component Headers
#include "../Engine/Transform.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/Camera.h"
#include "../Engine/Light.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Movement.h"
#include "../Engine/Script.h"
#include "../Engine/SampleScript.h"
#include "../Engine/StaticMesh.h"


using json = nlohmann::json;
namespace ECS { class Entity; }
GameEditor::GameEditor(HINSTANCE hInstance)
	:Engine(hInstance)
{
	//NLOHMANN_DEFINE_TYPE_INTRUSIVE
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
	
	m_EditorWorld = ECS::World::CreateWorld(L"TestScene1.json");
	m_ActiveWorld = m_EditorWorld;

	WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);

	/* ---- for test -------------------------------------------------------------------------- */
	{
		m_Box = m_EditorWorld->create();
		m_Pot = m_EditorWorld->create();
		m_Wall = m_EditorWorld->create();

		Vector3D pos1 = { 1.0f, 3.0f, 5.0f };
		Vector3D pos2 = { 10.0f, 30.0f, 50.0f };
		Vector3D pos3 = { 100.0f, 300.0f, 500.0f };
		m_Box->Assign<EntityIdentifier>(m_Box->getEntityId(), "Box");
		m_Pot->Assign<EntityIdentifier>(m_Pot->getEntityId(), "Pot");

		// 부모자식관계 안됨...
		SetParent(m_Pot, m_Box);
		m_Wall->Assign<EntityIdentifier>(m_Wall->getEntityId(), "Wall");
		m_Box->Assign<Transform>(pos1);
		m_Pot->Assign<Transform>(pos2);
		m_Wall->Assign<Transform>(pos3);
		m_Wall->Assign<Camera>();
	}
	/* ---- test end --------------------------------------------------------------------------- */


	//Test test;


	//// 이런식으로 변수 이름 가져와서 ImGui에서 컴포넌트들이 가지고 있는 멤버 변수들 출력할 수 있음
	//// 값은 어떻게 넣지?
	//for (const auto& a : test.GetTypeInfo().GetProperties())
	//{
	//	std::cout << a->GetName();
	//}

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
	m_Renderer->Instance->RenderBegin();
	m_Renderer->Instance->Render();
	RenderImGui();
	m_Renderer->Instance->RenderEnd();
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
					LoadWorld(L"TestScene1");	// Test

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save World"))
				{
					SaveWorld(L"TestScene1.json");
				}

				if (ImGui::MenuItem("Load World"))
				{
					LoadWorld(L"TestScene1.json");
				}

				if (ImGui::MenuItem("Exit"))
				{
					Close();
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
		ID3D11ShaderResourceView* myViewportTexture = Renderer::Instance->m_RenderTexture->GetShaderResourceView();
		ImGui::Image((void*)myViewportTexture, ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
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
void GameEditor::SaveWorld(const std::wstring& _filename)
{
	std::wstring fullPath = basePath + _filename;

	std::ofstream outputFile(fullPath);

	json worldData;
	for (const auto& entity : m_EditorWorld->GetEntities())
	{
		SaveComponents<EntityIdentifier>(entity, worldData, fullPath);
		SaveComponents<Transform>(entity, worldData, fullPath);
		SaveComponents<StaticMesh>(entity, worldData, fullPath);
		SaveComponents<BoxCollider>(entity, worldData, fullPath);
		SaveComponents<Camera>(entity, worldData, fullPath);
		SaveComponents<Light>(entity, worldData, fullPath);
		SaveComponents<Movement>(entity, worldData, fullPath);
		SaveComponents<Script>(entity, worldData, fullPath);


	}

	outputFile << std::setw(4) << worldData << std::endl;

	outputFile.close();

}

void GameEditor::LoadWorld(const std::wstring& _filename)
{
	m_EditorWorld = ECS::World::CreateWorld(_filename);

	std::wstring fullPath = basePath + _filename;

	std::ifstream inputFile(fullPath);
	json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	for (const auto& entity : jsonObject["WorldEntities"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			const int entityId = std::stoi(it.key());
			int tempId = entityId - 1;
			for (const auto& component : it.value())
			{
				auto myEntity = m_EditorWorld->create();
				if (myEntity->getEntityId() == entityId)
				{
					if (component.contains("EntityIdentifier"))
					{
						myEntity->Assign<EntityIdentifier>();

						const auto& Identifier = component["EntityIdentifier"][tempId];
						myEntity->get<EntityIdentifier>().get().m_EntityId = Identifier["m_EntityId"];
						myEntity->get<EntityIdentifier>().get().m_EntityName = Identifier["m_EntityName"];
					}

					if (component.contains("Transform"))
					{
						myEntity->Assign<Transform>();

						const auto& trans = component["Transform"][tempId];

						myEntity->get<Transform>().get().m_Position = trans["m_Position"];
						myEntity->get<Transform>().get().m_Rotation = trans["m_Rotation"];
						myEntity->get<Transform>().get().m_Scale = trans["m_Scale"];
					}

					if (component.contains("BoxCollider"))
					{
						myEntity->Assign<BoxCollider>();

						const auto& collider = component["BoxCollider"][tempId];
						myEntity->get<BoxCollider>().get().m_CurrentState = collider["m_CurrentStae"];
						myEntity->get<BoxCollider>().get().m_Center = collider["m_Center"];
						myEntity->get<BoxCollider>().get().m_Size = collider["m_Size"];
						myEntity->get<BoxCollider>().get().m_IsTrigger = collider["m_IsTrigger"];
					}

					if (component.contains("Camera"))
					{
						myEntity->Assign<Camera>();

						const auto& camera = component["Camera"][tempId];
						myEntity->get<Camera>().get().m_FOV = camera["m_FOV"];
						myEntity->get<Camera>().get().m_Near = camera["m_Near"];
						myEntity->get<Camera>().get().m_Far = camera["m_Far"];
					}

					if (component.contains("Light"))
					{
						myEntity->Assign<Light>();

						const auto& light = component["Light"];
						myEntity->get<Light>().get().m_Type = light["m_Type"];
						myEntity->get<Light>().get().m_Color = light["m_Color"];
						myEntity->get<Light>().get().m_Intensity = light["m_Intensity"];
					}

					if (component.contains("Movement"))
					{
						myEntity->Assign<Movement>();

						const auto& movement = component["Movement"];
						myEntity->get<Movement>().get().m_Speed = movement["m_Speed"];
						myEntity->get<Movement>().get().m_DirectionVector = movement["m_DirectionVector"];
					}

					if (component.contains("SampleScript"))
					{
						myEntity->Assign<SampleScript>();
					}

					if (component.contains("StaticMesh"))
					{
						myEntity->Assign<StaticMesh>();

						const auto& staticMesh = component["StaticMesh"];
						myEntity->get<StaticMesh>().get().m_FileName = staticMesh["m_FileName"];
					}
				}
			}
		}
	}

	/*for (const auto& data : deserializedTransform)
	{
		std::cout << "position : { " << data.m_Position.GetX() << data.m_Position.GetY() << data.m_Position.GetZ() << " }\n";
		std::cout << "rotation : { " << data.m_Rotation.GetX() << data.m_Rotation.GetY() << data.m_Rotation.GetZ() << " }\n";
		std::cout << "scale    : { " << data.m_Scale.GetX() << data.m_Scale.GetY() << data.m_Scale.GetZ() << " }\n";
	}*/
}

void GameEditor::NewScene()
{
	m_ActiveWorld = ECS::World::CreateWorld(L"TestScene1.json");
	m_SceneHierarchyPanel.SetContext(m_EditorWorld);

}

void GameEditor::SetParent(ECS::Entity* child, ECS::Entity* parent)
{
	child->get<EntityIdentifier>().get().m_ParentEntityId = parent->getEntityId();
	parent->childernId.push_back(child->getEntityId());
}
