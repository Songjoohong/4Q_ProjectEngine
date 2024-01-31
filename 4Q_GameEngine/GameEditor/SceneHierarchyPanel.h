#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>
#include "../Engine/ECS.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(ECS::World* context);

	void SetContext(ECS::World* context);

	void RenderImGui();

	ECS::Entity* GetSelectedEntity() const { return m_SelectionContext; }
	void SetSelectedEntity(ECS::Entity* entity) { m_SelectionContext = entity; }

	void DragDropEntity(ECS::Entity* entity);

private:
	template <typename T>
	void DisplayAddComponentEntry(const std::string& entryName);

	void DrawEntityNode(ECS::Entity* entity);
	void DrawComponents(ECS::Entity* entity);

private:
	ECS::World* m_Context;
	ECS::Entity* m_SelectionContext;		// TODO: 포인터로 가질지 실 객체로 가질지 고민좀..
};

template <typename T>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
{
	// 선택된 Entity 가 T타입의 component 를 가지고 있지 않다면
	if (!m_SelectionContext->has<T>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			// 현재 선택된 오브젝트에 컴포넌트 추가
			m_SelectionContext->Assign<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}
