#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>
#include "../Engine/ECS.h"
#include "../Engine/StaticMesh.h"

struct StaticMesh;
class PrefabManager;
class NameManager;
class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(ECS::World* context);

	void SetContext(ECS::World* context, std::shared_ptr<PrefabManager> prefab, std::shared_ptr<NameManager> nameManager);

	void RenderImGui();

	ECS::Entity* GetSelectedEntity() const { return m_SelectionContext; }
	void SetSelectedEntity(ECS::Entity* entity) { m_SelectionContext = entity; }
	void DeleteSelectedEntity() { m_Context->destroy(m_SelectionContext); m_SelectionContext = nullptr; }
	void SetPrefabFileName(ECS::Entity* entity);
	void DragDropEntityHierarchy(ECS::Entity* entity);

private:
	template <typename T>
	void DisplayAddComponentEntry(const std::string& entryName);
	template <>
	void DisplayAddComponentEntry<StaticMesh>(const std::string& entryName);

	void DrawEntityNode(ECS::Entity* entity);
	void DrawComponents(ECS::Entity* entity);

	void ShowStaticModelDialog();

	void SetParent(ECS::Entity* child, ECS::Entity* parent);

	bool m_IsDialogOpen = false;

private:
	ECS::World* m_Context;
	ECS::Entity* m_SelectionContext;
	ECS::Entity* picked;
	std::shared_ptr<PrefabManager> m_PrefabManager;
	std::shared_ptr<NameManager> m_NameManager;
	bool m_OpenTextPopup = false;
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

template <>
inline void SceneHierarchyPanel::DisplayAddComponentEntry<StaticMesh>(const std::string& entryName)
{
	// ���õ� Entity �� TŸ���� component �� ������ ���� �ʴٸ�
	if (!m_SelectionContext->has<StaticMesh>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			// TODO: StaticMesh ���̾�α�
			// �ҷ��� fbx ���� ����.
			// ���� ���̾�α� �޴� ��Ÿ��.
			// open Dialog Simple

			m_IsDialogOpen = true;

			// fbx ������ �������� �ʾҴٸ� ������Ʈ �߰����� ����.
			ImGui::CloseCurrentPopup();
		}
	}
}
