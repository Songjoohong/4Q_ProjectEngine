#pragma once

#include "../Engine/ECS.h"
class PrefabManager;
class ContentsBrowserPanel
{
public:
	ContentsBrowserPanel();

	void RenderImGui();
	void SetContext(ECS::World* world);
private:
	std::filesystem::path m_CurrentDirectory;
	ECS::World* m_World;
};

