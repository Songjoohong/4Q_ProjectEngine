#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>
#include "../Engine/ECS.h"

struct StaticMesh;

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(ECS::World* context);

	void SetContext(ECS::World* context);

	void RenderImGui();

	ECS::Entity* GetSelectedEntity() const { return m_SelectionContext; }
	void SetSelectedEntity(ECS::Entity* entity) { m_SelectionContext = entity; }

	void DragDropEntityHierarchy(ECS::Entity* entity);

private:
	template <typename T>
	void DisplayAddComponentEntry(const std::string& entryName);

	void DrawEntityNode(ECS::Entity* entity);
	void DrawComponents(ECS::Entity* entity);

private:
	ECS::World* m_Context;
	ECS::Entity* m_SelectionContext;
	ECS::Entity* picked;
};

template <typename T>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
{
	// ���õ� Entity �� TŸ���� component �� ������ ���� �ʴٸ�
	if (!m_SelectionContext->has<T>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			// ���� ���õ� ������Ʈ�� ������Ʈ �߰�
			m_SelectionContext->Assign<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}