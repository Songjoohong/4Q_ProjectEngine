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
	/// 씬이 두개인 이유
	///	게임 플레이와 씬 편집 화면을 나누기 위해. -> 게임 play 와 stop 그리고 pause 를 위해서인데 이를 위해선 엔진에서 먼저 기능이 구현되어야 한다. 고로 보류

	// TextEntities
	ECS::Entity* m_Box;
	ECS::Entity* m_Pot;
	ECS::Entity* m_Wall;
};