#pragma once
#include "ContentsBrowserPanel.h"
#include "SceneHierarchyPanel.h"
#include "../Engine/Engine.h"

class ContentsBrowserPanel;
class SceneHierarchyPanel;
class Renderer;

namespace ECS { class Entity; }
namespace ECS { class World; }

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
	void SaveWorld(const std::wstring& _strRelativePath);
	void LoadWorld(const std::wstring& _strRelativePath);

	void NewScene();
private:
	Renderer* m_Renderer = nullptr;

	std::wstring basePath = L"C:\\Users\\user\\Documents\\GitHub\\4Q_ProjectEngine\\4Q_GameEngine\\Test\\";

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentsBrowserPanel m_ContentsBrowserPanel;

	// Scenes
	ECS::World* m_ActiveWorld;
	ECS::World* m_EditorWorld;

	// TextEntities
	ECS::Entity* m_Box;
	ECS::Entity* m_Pot;
	ECS::Entity* m_Wall;
};