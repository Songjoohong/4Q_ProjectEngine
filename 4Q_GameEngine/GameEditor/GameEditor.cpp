#include "pch.h"
#include "GameEditor.h"
#include "framework.h"
#include "../D3D_Graphics/D3D_Graphics.h"
#include 
GameEditor::GameEditor(HINSTANCE hInstance)
	:Engine(hInstance)
{
	m_Renderer = Renderer::Instance;
}

GameEditor::~GameEditor()
{
	delete m_Renderer;
}

bool GameEditor::Initialize(UINT width, UINT height)
{
	return false;
}

void GameEditor::Update()
{
}

void GameEditor::Render()
{
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
