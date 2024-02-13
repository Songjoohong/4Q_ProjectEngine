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

	template<typename ComponentType>
	void AssignComponents(ECS::Entity* entity, json& componentData);
	void NewScene();

	void SetParent(ECS::Entity* child, ECS::Entity* parent);

	std::shared_ptr<PrefabManager> m_PrefabManager;
	std::shared_ptr< NameManager> m_NameManager;

	Vector3D m_TranslationSnapValue = {	4.0f, 4.0f, 4.0f };
	Vector3D m_RotationSnapValue = { 1.0f, 1.0f, 1.0f };
	Vector3D m_ScaleSnapValue = { 1.0f, 1.0f, 1.0f };

	Vector3D* m_CurrentSnapMode = &m_TranslationSnapValue;
private:
	Renderer* m_Renderer = nullptr;

	std::string basePath = "../Test/";

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentsBrowserPanel m_ContentsBrowserPanel;

	// Scenes
	ECS::World* m_ActiveWorld;
	ECS::World* m_EditorWorld;
	/// 씬이 두개인 이유
	///	게임 플레이와 씬 편집 화면을 나누기 위해. -> 게임 play 와 stop 그리고 pause 를 위해서인데 이를 위해선 엔진에서 먼저 기능이 구현되어야 한다. 고로 보류

	// gizmo
	int m_GizmoType = 0;

	// TextEntities
	ECS::Entity* m_Camera;
	ECS::Entity* m_Box;
	ECS::Entity* m_Pot;
	ECS::Entity* m_Wall;
};

template<typename ComponentType>
inline void GameEditor::SaveComponents(ECS::Entity* entity, json& worldData)
{
	if (std::is_base_of_v <Script, ComponentType>)
	{
		std::vector<ComponentType> container;
		container.push_back(entity->get<ComponentType>().get());
		auto ScriptData = SerializeContainer(container);

		json componentData;
		componentData[(entity->get<ComponentType>().get()).m_ComponentName] = json::parse(ScriptData);

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
inline void GameEditor::AssignComponents(ECS::Entity* entity, json& componentData)
{
	if constexpr (std::is_base_of_v<Script, ComponentType>)
	{
		entity->Assign<ComponentType>();
	}
	else if (std::is_same_v<StaticMesh, ComponentType>)
	{
		entity->Assign<ComponentType>();
	}
	else
	{
		entity->Assign<ComponentType>();
	}

	auto& component = entity->get<ComponentType>().get();

	component = componentData;
}
