#pragma once
#include "Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

public:
	void OnAttach() override;
	void OnDetach() override;

	void Begin();
	void End();
	void SetDarkThemeColors();
	void SetDarkThemeV2Colors();
	//void OnEvent();
	void ImGuiShutDown();
};