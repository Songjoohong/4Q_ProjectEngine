#pragma once
#include "ContentsBrowserPanel.h"
#include "SceneHierarchyPanel.h"
#include "../Engine/Engine.h"

class ContentsBrowserPanel;
class SceneHierarchyPanel;
class Renderer;
class EntityIdentifier;
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
	void SaveWorld(const std::wstring& _strRelativePath);
	template<typename ComponentType>
	void SaveComponents(ECS::Entity* entity, json& worldData, std::wstring& filename);
	void LoadWorld(const std::wstring& _strRelativePath);

	template<typename ComponentType>
	void AssignComponents(ECS::Entity* entity, const json& componentData);
	void NewScene();

	void SetParent(ECS::Entity* child, ECS::Entity* parent);

private:
	Renderer* m_Renderer = nullptr;

	std::wstring basePath = L"../Test/";

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentsBrowserPanel m_ContentsBrowserPanel;

	// Scenes
	ECS::World* m_ActiveWorld;
	ECS::World* m_EditorWorld;
	/// ���� �ΰ��� ����
	///	���� �÷��̿� �� ���� ȭ���� ������ ����. -> ���� play �� stop �׸��� pause �� ���ؼ��ε� �̸� ���ؼ� �������� ���� ����� �����Ǿ�� �Ѵ�. ���� ����

	// TextEntities
	ECS::Entity* m_Camera;
	ECS::Entity* m_Box;
	ECS::Entity* m_Pot;
	ECS::Entity* m_Wall;
};

template<typename ComponentType>
inline void GameEditor::SaveComponents(ECS::Entity* entity, json& worldData, std::wstring& filename)
{
	if (entity->has<ComponentType>())
	{
		std::vector<ComponentType> container;
		container.push_back(entity->get<ComponentType>().get());
		auto serializedData = SerializeContainer(container, filename);

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
	entity->Assign<ComponentType>();

	auto& component = entity->get<ComponentType>().get();

	component = componentData;
}