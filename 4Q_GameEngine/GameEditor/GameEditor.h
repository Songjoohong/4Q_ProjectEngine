#pragma once
#include "../Engine/Engine.h"

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

	//void SaveScene(const wstring& _strRelativePath);
	//void LoadScene(const wstring& _strRelativePath);
private:
	Renderer* m_Renderer = nullptr;
};