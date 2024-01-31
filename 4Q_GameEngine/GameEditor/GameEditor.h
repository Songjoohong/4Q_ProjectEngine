#pragma once
#include "../Engine/Engine.h"
class Renderer;
namespace ECS { class World; }
namespace ECS { class Entity; }
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
	void SaveWorld(const std::wstring& _filename);
	void LoadWorld(const std::wstring& _filename);
private:
	Renderer* m_Renderer = nullptr;
	ECS::World* m_CurrentWorld = nullptr;
	ECS::World* m_ActiveWorld = nullptr;
	ECS::Entity* m_Box = nullptr;
	ECS::Entity* m_Pot = nullptr;
	ECS::Entity* m_Wall = nullptr;
	std::wstring basePath = L"C:\\Users\\user\\Documents\\GitHub\\4Q_ProjectEngine\\4Q_GameEngine\\Test\\MyScene\\";


};

