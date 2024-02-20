#pragma once
#include "ContentsBrowserPanel.h"
#include "SceneHierarchyPanel.h"
#include "../Engine/Engine.h"
#include "../Engine/Vector3D.h"
#include <codecvt>
class ContentsBrowserPanel;
class SceneHierarchyPanel;
class Renderer;
class EntityIdentifier;
class Script;
class PrefabManager;
class NameManager;
class FreeCameraScript;
class BoxCollider;
class Sprite2D;

namespace ECS { class Entity; }
namespace ECS { class World; }

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
	void SetDarkThemeColors();

	//Save/Load
	void SaveWorld(const std::string& _strRelativePath);
	template<typename ComponentType>
	void SaveComponents(ECS::Entity* entity, json& worldData);
	void LoadWorld(const std::string& _strRelativePath);
	void ShowSceneDialog();
	void ShowSaveSceneAsPopup();

	void PlayDeserialize(ECS::World* currentWorld, const std::string& _filename);
	void Deserialize(ECS::World* currentWorld, const std::string& fileName);

	void PlayButton();
	// 단축키 설정
	void HandleShortcuts();

	template<typename ComponentType>
	void AssignComponents(ECS::Entity* entity, const json& componentData);
	void NewScene();
	void PlayScene();
	void SetParent(ECS::Entity* child, ECS::Entity* parent);
	void SetParentTransform(ECS::Entity* child, ECS::Entity* parent);

	std::shared_ptr<PrefabManager> m_PrefabManager;
	std::shared_ptr< NameManager> m_NameManager;

	Vector3D m_TranslationSnapValue = {	4.0f, 4.0f, 4.0f };
	Vector3D m_RotationSnapValue = { 1.0f, 1.0f, 1.0f };
	Vector3D m_ScaleSnapValue = { 1.0f, 1.0f, 1.0f };

	Vector3D* m_CurrentSnapMode = &m_TranslationSnapValue;
private:

	std::string basePath = "../Resource/";

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentsBrowserPanel m_ContentsBrowserPanel;

	// Scenes
	ECS::World* m_ActiveWorld;
	ECS::World* m_EditorWorld;
	/// 씬이 두개인 이유
	///	게임 플레이와 씬 편집 화면을 나누기 위해. -> 게임 play 와 stop 그리고 pause 를 위해서.
	std::string m_SceneName = "EmptyScene";	// 씬 이름
	bool m_isScenePopup = false;

	// dialog
	bool m_IsDialogOpen = false;

	// gizmo
	int m_GizmoType = 0;
	float TranslationSnapValue = 10.0f;
	int	intTransSnapValue = 0;
	int	intRotSnapValue = 0;
	int	intScaleSnapValue = 0;

	// play
	bool m_IsPlaying = false;

	// Application Height & Width
	UINT m_Width;
	UINT m_Height;

	ID3D11ShaderResourceView* m_PlayButtonTexture;

	std::vector<std::pair<ECS::Entity*, int>> m_LoadEntityContainer;
};

template<typename ComponentType>
inline void GameEditor::SaveComponents(ECS::Entity* entity, json& worldData)
{
	if (std::is_same_v < Script, ComponentType> && entity->has<ComponentType>())
	{
		std::vector<ComponentType> container;
		container.push_back(entity->get<ComponentType>().get());
		auto serializeData = SerializeContainer(container);

		json scriptData;
		scriptData["Script"] = json::parse(serializeData);

		std::string entityName = entity->get<EntityIdentifier>().get().m_EntityName;

		// Check if the entity already exists in the JSON structure
		bool entityExists = false;
		for (auto& entityEntry : worldData["WorldEntities"]) {
			if (entityEntry.find(entityName) != entityEntry.end()) {
				// Add the component data to the existing entity entry
				entityEntry[entityName].push_back(scriptData);
				entityExists = true;
				break;
			}
		}

		// If the entity does not exist, create a new entry for it
		if (!entityExists) {
			json entityEntry;
			entityEntry[entityName].push_back(scriptData);
			worldData["WorldEntities"].push_back(entityEntry);
		}
	}
	else if (entity->has<ComponentType>())
	{
		std::vector<ComponentType> container;
		container.push_back(entity->get<ComponentType>().get());
		auto serializedData = SerializeContainer(container);

		json componentData;
		componentData[(entity->get<ComponentType>().get()).m_ComponentName] = json::parse(serializedData);

		std::string entityName = entity->get<EntityIdentifier>().get().m_EntityName;

		// Check if the entity already exists in the JSON structure
		bool entityExists = false;
		for (auto& entityEntry : worldData["WorldEntities"]) {
			if (entityEntry.find(entityName) != entityEntry.end()) {
				// Add the component data to the existing entity entry
				entityEntry[entityName].push_back(componentData);
				entityExists = true;
				break;
			}
		}

		// If the entity does not exist, create a new entry for it
		if (!entityExists) {
			json entityEntry;
			entityEntry[entityName].push_back(componentData);
			worldData["WorldEntities"].push_back(entityEntry);
		}
		std::cout << serializedData << std::endl;
	}
}

template<typename ComponentType>
inline void GameEditor::AssignComponents(ECS::Entity* entity, const json& componentData)
{
	if constexpr (std::is_base_of_v<Script, ComponentType>)
	{
		if (componentData["m_ComponentName"].get<std::string>() == "FreeCameraScript")
		{
			entity->Assign<FreeCameraScript>(entity);
			entity->get<Script>()->m_ComponentName = componentData["m_ComponentName"].get<std::string>();
		}
		else
		{
			entity->Assign<ComponentType>(entity);
			entity->get<Script>()->m_ComponentName = componentData["m_ComponentName"].get<std::string>();
		}
	}
	else if constexpr (std::is_same_v<Sprite2D, ComponentType>)
	{
		entity->Assign<ComponentType>(componentData["m_FileName"].get<std::string>());
		auto& component = entity->get<ComponentType>().get();

		component = componentData;
	}
	else if constexpr (std::is_same_v<BoxCollider, ComponentType>)
	{
		entity->Assign<ComponentType>(componentData["m_ColliderType"], componentData["m_CollisionType"], componentData["m_Size"]);
		auto& component = entity->get<ComponentType>().get();

		component = componentData;
	}
	else
	{
		entity->Assign<ComponentType>();
		auto& component = entity->get<ComponentType>().get();

		component = componentData;
	}
}
