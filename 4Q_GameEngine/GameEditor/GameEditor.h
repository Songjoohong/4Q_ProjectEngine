#pragma once
#include "ContentsBrowserPanel.h"
#include "SceneHierarchyPanel.h"
#include "../Engine/Engine.h"

class ContentsBrowserPanel;
class SceneHierarchyPanel;
class Renderer;
class GameEditor : public Engine
{
public:
	GameEditor(HINSTANCE hInstance);
	virtual ~GameEditor() override;

public:
	virtual bool Initialize(UINT width, UINT height) override;
	virtual void Update();
	virtual void Render();

	//ImGui
	bool InitImGui();
	void BeginRenderImGui();
	void RenderImGui();
	void EndRenderImGui();
	void ShutDownImGui();

	//Save/Load
	void SaveScene(const std::wstring& _strRelativePath);
	void LoadScene(const std::wstring& _strRelativePath);

	void NewScene();
private:
	Renderer* m_Renderer = nullptr;

	std::wstring basePath = L"C:\\Users\\user\\Documents\\GitHub\\4Q_ProjectEngine\\4Q_GameEngine\\Test\\";

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentsBrowserPanel m_ContentsBrowserPanel;

	// Scenes
	ECS::World* m_ActiveScene;
	ECS::World* m_EditorScene;
};