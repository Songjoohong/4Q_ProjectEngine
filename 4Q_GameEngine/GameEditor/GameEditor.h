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
private:
	Renderer* m_Renderer;
};