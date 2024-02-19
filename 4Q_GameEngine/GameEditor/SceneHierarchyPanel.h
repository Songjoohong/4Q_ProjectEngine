#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>

#include "../Engine/BoxCollider.h"
#include "../Engine/ECS.h"
#include "../Engine/Sprite2D.h"
#include "../Engine/StaticMesh.h"

struct Sprite2D;
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
	template <>
	inline void DisplayAddComponentEntry<Sprite2D>(const std::string& entryName);

	void DrawEntityNode(ECS::Entity* entity);
	void DrawComponents(ECS::Entity* entity);

	void ShowStaticModelDialog();
	template <typename T>
	void ShowDialog();	// TODO: ShowStaticModelDialog 함수를 템플릿화할 것이다.

	void SetParent(ECS::Entity* child, ECS::Entity* parent);

	void ResetTransform(ECS::Entity* child, ECS::Entity* parent);
	bool m_IsDialogOpen = false;
	bool m_AssignStaticMesh = false;
	bool m_AssignSprite2D = false;

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
	// 이미 있다면 추가하지 않음
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
	// 이미 있다면 추가하지 않음
	if (!m_SelectionContext->has<StaticMesh>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			// 불러올 fbx 파일 선택하는 다이얼로그를 나타내는 트리거 on
			m_IsDialogOpen = true;
			m_AssignStaticMesh = true;
			ImGui::CloseCurrentPopup();
		}
	}
}

template <>
inline void SceneHierarchyPanel::DisplayAddComponentEntry<Sprite2D>(const std::string& entryName)
{
	// 이미 있다면 추가하지 않음
	if (!m_SelectionContext->has<Sprite2D>())
	{
		if (ImGui::MenuItem(entryName.c_str()))
		{
			// 불러올 ui 파일 선택하는 다이얼로그를 나타내는 트리거 on
			m_IsDialogOpen = true;
			m_AssignSprite2D = true;
			ImGui::CloseCurrentPopup();
		}
	}
}

template <typename T>
void SceneHierarchyPanel::ShowDialog()
{
	std::string fileName;
	std::string resourceDirectoryName = "";



	if (m_IsDialogOpen)
	{
		if constexpr (std::is_same_v<T, StaticMesh>)
		{
			resourceDirectoryName = "fbx";
			IGFD::FileDialogConfig config; config.path = "../Resource/" + resourceDirectoryName;
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ("." + resourceDirectoryName).c_str(), config);
		}
		else if constexpr (std::is_same_v<T, Sprite2D>)
		{
			resourceDirectoryName = "UI";
			IGFD::FileDialogConfig config; config.path = "../Resource/" + resourceDirectoryName;
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png,.jpg", config);
		}
		else
			return;
	}
	else
	{
		return;
	}

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

			// action

			if (std::is_same_v<T, Sprite2D>)
				m_SelectionContext->Assign<T>("../Resource/" + resourceDirectoryName + "/" + fileName);
			else
				m_SelectionContext->Assign<T>(resourceDirectoryName + "/" + fileName);
			
		}

		// close
		ImGuiFileDialog::Instance()->Close();
		m_IsDialogOpen = false;
		m_AssignSprite2D = false;
		m_AssignStaticMesh = false;
	}
}
