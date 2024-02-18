#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>
#include "../Engine/ECS.h"
#include "../Engine/StaticMesh.h"

struct BoxCollider;
struct StaticMesh;
class PrefabManager;
class NameManager;

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(ECS::World* context);

	~SceneHierarchyPanel();

	void SetContext(ECS::World* context, std::shared_ptr<PrefabManager> prefab, std::shared_ptr<NameManager> nameManager);

	void RenderImGui();

	ECS::Entity* GetSelectedEntity() const { return m_SelectionContext; }
	void SetSelectedEntity(ECS::Entity* entity) { m_SelectionContext = entity; }
	void DeleteSelectedEntity() { m_Context->destroy(m_SelectionContext); m_SelectionContext = nullptr; }
	void SetPrefabFileName(ECS::Entity* entity);
	void DragDropEntityHierarchy(ECS::Entity* entity);

	bool FileExists(const std::string& filename);
private:
	template <typename T>
	inline void DisplayAddComponentEntry(const std::string& entryName);
	template <>
	inline void DisplayAddComponentEntry<StaticMesh>(const std::string& entryName);

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


	std::vector<ECS::Entity*> deletedEntities;
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

template <>
void SceneHierarchyPanel::DisplayAddComponentEntry<StaticMesh>(const std::string& entryName)
{
	// 선택된 Entity 가 T타입의 component 를 가지고 있지 않다면
	if (!m_SelectionContext->has<StaticMesh>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			// TODO: StaticMesh 다이얼로그
			// 불러올 fbx 파일 선택.
			// 파일 다이어로그 메뉴 나타냄.
			// open Dialog Simple

			m_IsDialogOpen = true;

			// fbx 파일을 선택하지 않았다면 컴포넌트 추가하지 않음.
			ImGui::CloseCurrentPopup();
		}
	}
}
