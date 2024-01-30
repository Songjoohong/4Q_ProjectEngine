#pragma once
#include "../Engine/ECS.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const std::shared_ptr<ECS::World>& world);

	void SetContext(const std::shared_ptr <ECS::World>& world);

	void RenderImGui();

	ECS::Entity GetSelectedEntity() const { m_SelectionContext; }
	void SetSelectedEntity(ECS::Entity entity);

private:
	std::shared_ptr<ECS::World> m_Context;
	ECS::Entity m_SelectionContext;
};

