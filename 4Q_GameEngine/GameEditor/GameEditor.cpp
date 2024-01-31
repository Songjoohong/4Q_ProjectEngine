#include "pch.h"
#include "GameEditor.h"
#include "framework.h"
#include "../D3D_Graphics/D3D_Graphics.h"

#include "Test.h"
#include "jsonSerializer.h"
#include "nlohmann/json.hpp"

#include "../D3D_Graphics/RenderTextureClass.h"


#include "ContentsBrowserPanel.h"
#include "SceneHierarchyPanel.h"

GameEditor::GameEditor(HINSTANCE hInstance)
	:Engine(hInstance)
{
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
	
	//NLOHMANN_DEFINE_TYPE_INTRUSIVE

	Test test;

	// 이런식으로 변수 이름 가져와서 ImGui에서 컴포넌트들이 가지고 있는 멤버 변수들 출력할 수 있음
	// 값은 어떻게 넣지?
	for (const auto& a : test.GetTypeInfo().GetProperties())
	{
		std::cout << a->GetName();
	}

	//SaveScene(L"w");

	if (!InitImGui())
	{
		return false;
	}

	m_EditorScene = ECS::World::CreateWorld("filenameSample1.txt");
	m_ActiveScene = m_EditorScene;

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
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					LoadScene(L"MyScene\\TestScene1");	// Test

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene(L"MyScene\\TestScene1");	// Test

				if (ImGui::MenuItem("Exit"))
					Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.RenderImGui();
		m_ContentsBrowserPanel.RenderImGui();

		//ImGui::ShowDemoWindow();	// for test

		ImGui::End();

		/* Viewport 렌더 ------------------------ */
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
void GameEditor::SaveScene(const std::wstring& _filename )
{

	std::wstring fullPath = basePath + _filename;

	FILE* pFile = nullptr;

	//"wb"는 파일을 와이드 문자 바이너리 모드로 열겠다는 의미
	_wfopen_s(&pFile, fullPath.c_str(), L"wb");

	assert(pFile != nullptr && "Filepath is invalid, couldn't save");

	// 데이터 저장
	

	fclose(pFile);
}

void GameEditor::LoadScene(const std::wstring& _strRelativePath)
{
	std::wstring fullPath = basePath + _strRelativePath;

	FILE* pFile = nullptr;

	// "rb"는 파일을 와이드 문자 바이너리 모드로 읽겠다는 의미
	_wfopen_s(&pFile, fullPath.c_str(), L"rb");

	assert(pFile != nullptr && "Filepath is invalid, couldn't load");

	fclose(pFile);
}

void GameEditor::NewScene()
{
	m_ActiveScene = ECS::World::CreateWorld("filenameSample2.txt");
	m_SceneHierarchyPanel.SetContext(m_ActiveScene);

}
