#pragma once
#include "../Engine/ECS.h"

namespace ECS { class Entity; }
namespace ECS { class World; }

class EntityIdentifier;

using json = nlohmann::json;

class PrefabManager
{
public:
	PrefabManager();
	~PrefabManager();

public:
	void SavePrefab(ECS::Entity* _selectedEntity, const std::string& _filename);
    ECS::Entity* LoadPrefab(const std::string& _filename);
	void DeleteAllDataInJsonFile(const std::string& filename);
	template<typename ComponentType>
	void SaveComponents(ECS::Entity* entity, json& prefabData);
    void RecursiveSaveComponents(ECS::Entity* entity, json& prefabData);

public:
	std::string basePath = "../Test/";
    json prefabData;
};

template<typename ComponentType>
inline void PrefabManager::SaveComponents(ECS::Entity* entity, json& prefabData)
{
    if (entity->has<ComponentType>())
    {
        std::vector<ComponentType> container;
        container.push_back(entity->get<ComponentType>().get());
        auto serializedData = SerializeContainer(container);

        // 컴포넌트 멤버 변수들 집어넣기
        json componentData;
        componentData[(entity->get<ComponentType>().get()).m_ComponentName] = json::parse(serializedData);

        std::string entityName = entity->get<EntityIdentifier>().get().m_EntityName;

        // Check if the "Prefabs" key exists and create it if not
        if (!prefabData.contains("Prefabs")) {
            prefabData["Prefabs"] = json::object();
        }

        // Check if the entity already exists in the JSON structure
        if (prefabData["Prefabs"].contains(entityName)) {
            prefabData["Prefabs"][entityName].push_back(componentData);
        }
        else {
            json entityEntry;
            entityEntry.push_back(componentData);
            prefabData["Prefabs"][entityName] = entityEntry;
        }
    }
}