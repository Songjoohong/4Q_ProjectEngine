#include "pch.h"
#include "GameEditor.h"
#include "framework.h"
#include "../D3D_Graphics/D3D_Graphics.h"

#include "Test.h"
#include "../ReflectionLib/jsonSerializer.h"
#include "../D3D_Graphics/RenderTextureClass.h"
#include "../Engine/ECS.h"
#include "../Engine/Transform.h"
#include "../Engine/EntityIdentifer.h"
using json = nlohmann::json;

GameEditor::GameEditor(HINSTANCE hInstance)
	:Engine(hInstance)
{
	//NLOHMANN_DEFINE_TYPE_INTRUSIVE
}

GameEditor::~GameEditor()
{
	delete m_Renderer;
	ShutDownImGui();
}

bool GameEditor::Initialize(UINT width, UINT height)
{
	__super::Initialize(width, height);
	m_Renderer = Renderer::Instance;
	
	m_EditorWorld = ECS::World::CreateWorld(L"TestScene1.json");
	m_Box = m_EditorWorld->create();
	m_Pot = m_EditorWorld->create();
	m_Wall = m_EditorWorld->create();

	Vector3D pos1 = { 1.0f, 3.0f, 5.0f };
	Vector3D pos2 = { 10.0f, 30.0f, 50.0f };
	Vector3D pos3 = { 100.0f, 300.0f, 500.0f };
	m_Box->Assign<EntityIdentifer>(m_Box->getEntityId(), "Box");
	m_Pot->Assign<EntityIdentifer>(m_Pot->getEntityId(), "Pot");

	m_Pot->SetParent(m_Box);
	m_Wall->Assign<EntityIdentifer>(m_Wall->getEntityId(), "Wall");
	m_Box->Assign<Transform>(m_Box->getEntityId(), pos1);
	m_Pot->Assign<Transform>(m_Pot->getEntityId(), pos2);
	m_Wall->Assign<Transform>(m_Wall->getEntityId(), pos3);
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
	__super::Render();
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

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					Close();
				}

				if (ImGui::MenuItem("Save"))
				{
					SaveWorld(L"TestScene1.json");
					LoadWorld(L"TestScene1.json");
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

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



// jsonFile 이름 넘기기
void GameEditor::SaveWorld(const std::wstring& _filename)
{
	std::wstring fullPath = basePath + _filename;

	std::ofstream outputFile(fullPath);
	json jsonObject;
	outputFile << jsonObject;
	outputFile.close();
	std::string serializedTransform;

	/*for (const auto& entity : m_CurrentWorld->GetEntities())
	{
		entity->get<Transform>().get();
		serializedTransform = SerializeContainer();
	}*/

	std::cout << serializedTransform << std::endl;
}

void GameEditor::LoadWorld(const std::wstring& _filename)
{
	m_EditorWorld = ECS::World::CreateWorld(_filename);
	m_ActiveWorld = m_EditorWorld;

	std::wstring fullPath = basePath + _filename;

	auto deserializedTransform = DeserializeContainerFromFile<std::array<Transform, 3>>(fullPath);

	for (const auto& data : deserializedTransform)
	{
		std::cout << "position : { " << data.m_Position.GetX() << data.m_Position.GetY() << data.m_Position.GetZ() << " }\n";
		std::cout << "rotation : { " << data.m_Rotation.GetX() << data.m_Rotation.GetY() << data.m_Rotation.GetZ() << " }\n";
		std::cout << "scale    : { " << data.m_Scale.GetX() << data.m_Scale.GetY() << data.m_Scale.GetZ() << " }\n";
	}
}
