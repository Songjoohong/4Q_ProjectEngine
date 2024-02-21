#pragma once
#include "../Engine/ECS.h"
#include "../Engine/EntityIdentifier.h"

namespace ECS { class Entity; }
namespace ECS { class World; }

class EntityIdentifier;
class NameManager;

class Script;
class FreeCameraScript;
class Sprite2D;
class BoxCollider;
class Sprite2D;
using json = nlohmann::json;

class PrefabManager
{
public:
	PrefabManager(ECS::World* currentWorld, std::shared_ptr<NameManager> nameManager);
	~PrefabManager();

public:
	void SavePrefab(ECS::Entity* _selectedEntity, const std::string& _filename);
    ECS::Entity* LoadPrefab(const std::string& _filename);

    void RecursiveSaveComponents(ECS::Entity* entity, json& prefabData);

	template<typename ComponentType>
	void SaveComponents(ECS::Entity* entity, json& prefabData);
    //template<>
    //inline void SaveComponents<EntityIdentifier>(ECS::Entity* entity, json& prefabData);
    template<typename ComponentType>
    void AssignComponents(ECS::Entity* entity, const json& componentData);

    void SetParent(ECS::Entity* child, ECS::Entity* parent);
	void DeleteAllDataInJsonFile(const std::string& filename);

    void SetContext(ECS::World* world);

    ECS::Entity* FindEntityByName(std::string entityName);
public:
	std::string basePath = "/prefab/";
    json prefabData;

    ECS::World* m_CurrentWorld;
    std::shared_ptr<NameManager> m_NameManager;
    std::vector<std::pair<ECS::Entity*,int>> m_prefabContainer;
};

template<typename ComponentType>
inline void PrefabManager::SaveComponents(ECS::Entity* entity, json& prefabData)
{
    if (entity->has<ComponentType>())
    {
        std::vector<ComponentType> container;
        container.push_back(entity->get<ComponentType>().get());
        auto serializedData = SerializeContainer(container);

        // ������Ʈ ��� ������ ����ֱ�
        json componentData;
        componentData[(entity->get<ComponentType>().get()).m_ComponentName] = json::parse(serializedData);

        std::string entityName = entity->get<EntityIdentifier>().get().m_EntityName;

        // Check if the "Prefabs" key exists and create it if not
        // Check if the entity already exists in the JSON structure
        bool entityExists = false;
        for (auto& entityEntry : prefabData["Prefabs"]) {
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
            prefabData["Prefabs"].push_back(entityEntry);
        }
    }
}

template<typename ComponentType>
inline void PrefabManager::AssignComponents(ECS::Entity* entity, const json& componentData)
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
    else if constexpr (std::is_same_v<Sprite2D, ComponentType>)
    {
        entity->Assign<ComponentType>(componentData["m_FileName"].get<std::string>(), componentData["m_Layer"], componentData["m_Position"][0], componentData["m_Position"][1]);
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